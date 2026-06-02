#include "Editor.h"
#include <QFileInfo>
#include <QTemporaryFile>
#include <QDir>

Editor::Editor(QWidget *parent)
    : QMainWindow(parent), isDarkTheme(true), isModified(false)
{
    themeManager   = new ThemeManager();
    compileProcess = new QProcess(this);

    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupOutputPanel();
    loadSettings();
    applyTheme();

    autoSaveTimer = new QTimer(this);
    connect(autoSaveTimer, &QTimer::timeout, this, [this]() {
        CodeEditor *ed = currentEditor();
        if (ed && !ed->filePath.isEmpty() && ed->document()->isModified()) {
            saveFile();
            statusLabel->setText("Auto Saved");
        }
    });
    autoSaveTimer->start(30000);

    connect(compileProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        outputPanel->appendPlainText(compileProcess->readAllStandardOutput());
    });
    connect(compileProcess, &QProcess::readyReadStandardError, this, [this]() {
        outputPanel->appendPlainText(compileProcess->readAllStandardError());
    });

    setWindowTitle("Code Editor");
    resize(1200, 750);
}

Editor::~Editor() { delete themeManager; }

CodeEditor* Editor::currentEditor() {
    return qobject_cast<CodeEditor*>(tabWidget->currentWidget());
}

void Editor::setupUI() {
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &Editor::closeTab);
    setCentralWidget(tabWidget);
    newTab();
}

void Editor::newTab() {
    CodeEditor *ed = new CodeEditor(this);
    connect(ed->document(), &QTextDocument::modificationChanged,
            this, &Editor::documentModified);
    connect(ed, &CodeEditor::cursorPositionChanged,
            this, &Editor::updateStatusBar);
    int idx = tabWidget->addTab(ed, "Untitled");
    tabWidget->setCurrentIndex(idx);
}

void Editor::closeTab(int index) {
    CodeEditor *ed = qobject_cast<CodeEditor*>(tabWidget->widget(index));
    if (ed && ed->document()->isModified()) {
        auto btn = QMessageBox::warning(this, "Unsaved Changes",
            "Save before closing this tab?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (btn == QMessageBox::Cancel) return;
        if (btn == QMessageBox::Save) {
            tabWidget->setCurrentIndex(index);
            saveFile();
        }
    }
    tabWidget->removeTab(index);
    if (tabWidget->count() == 0) newTab();
}

void Editor::setupOutputPanel() {
    outputDock  = new QDockWidget("Output", this);
    outputPanel = new QPlainTextEdit(outputDock);
    outputPanel->setReadOnly(true);
    outputPanel->setMaximumHeight(180);
    QFont f("Cascadia Code", 11);
    f.setStyleHint(QFont::Monospace);
    outputPanel->setFont(f);
    outputDock->setWidget(outputPanel);
    outputDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, outputDock);
    outputDock->hide();
}

void Editor::setupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("New Tab",   this, &Editor::newTab,     QKeySequence::New);
    fileMenu->addAction("Open...",   this, &Editor::openFile,   QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction("Save",      this, &Editor::saveFile,   QKeySequence::Save);
    fileMenu->addAction("Save As...",this, &Editor::saveFileAs, QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close, QKeySequence::Quit);

    QMenu *editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction("Undo",  this, [this](){ if(currentEditor()) currentEditor()->undo(); }, QKeySequence::Undo);
    editMenu->addAction("Redo",  this, [this](){ if(currentEditor()) currentEditor()->redo(); }, QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction("Cut",   this, [this](){ if(currentEditor()) currentEditor()->cut(); },  QKeySequence::Cut);
    editMenu->addAction("Copy",  this, [this](){ if(currentEditor()) currentEditor()->copy(); }, QKeySequence::Copy);
    editMenu->addAction("Paste", this, [this](){ if(currentEditor()) currentEditor()->paste(); },QKeySequence::Paste);
    editMenu->addSeparator();
    editMenu->addAction("Find & Replace...", this, &Editor::showFindReplace, QKeySequence::Find);

    QMenu *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Toggle Dark/Light Theme", this, &Editor::toggleTheme,
                        QKeySequence(Qt::CTRL | Qt::Key_T));
    viewMenu->addAction("Change Font...", this, &Editor::changeFont);
    viewMenu->addAction("Toggle Output Panel", this, [this](){
        outputDock->setVisible(!outputDock->isVisible());
    });

    QMenu *runMenu = menuBar()->addMenu("&Run");
    runMenu->addAction("▶  Run Code", this, &Editor::runCode, QKeySequence(Qt::CTRL | Qt::Key_R));
}

void Editor::setupToolBar() {
    toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);
    toolBar->addAction("New",   this, &Editor::newTab);
    toolBar->addAction("Open",  this, &Editor::openFile);
    toolBar->addAction("Save",  this, &Editor::saveFile);
    toolBar->addSeparator();
    toolBar->addAction("Find",  this, &Editor::showFindReplace);
    toolBar->addSeparator();
    toolBar->addAction("▶ Run", this, &Editor::runCode);
    toolBar->addSeparator();
    toolBar->addAction("Theme", this, &Editor::toggleTheme);
    toolBar->addSeparator();
    toolBar->addWidget(new QLabel("  Size: "));
    fontSizeBox = new QComboBox(this);
    for (int sz : {10,11,12,13,14,16,18,20,24,28,32})
        fontSizeBox->addItem(QString::number(sz), sz);
    fontSizeBox->setCurrentText("14");
    connect(fontSizeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int idx){ changeFontSize(fontSizeBox->itemData(idx).toInt()); });
    toolBar->addWidget(fontSizeBox);
}

void Editor::setupStatusBar() {
    statusLabel  = new QLabel("Ready");
    lineColLabel = new QLabel("Ln 1, Col 1");
    fileLabel    = new QLabel("Untitled");
    statusBar()->addWidget(statusLabel, 1);
    statusBar()->addPermanentWidget(fileLabel);
    statusBar()->addPermanentWidget(lineColLabel);
}

void Editor::runCode() {
    CodeEditor *ed = currentEditor();
    if (!ed) return;

    // Save first
    if (ed->document()->isModified() || ed->filePath.isEmpty()) {
        if (!saveFile()) return;
    }

    QString filePath = ed->filePath;
    if (!filePath.endsWith(".cpp") && !filePath.endsWith(".c")) {
        outputPanel->setPlainText("⚠ Only C/C++ files can be compiled.");
        outputDock->show();
        return;
    }

    outputDock->show();
    outputPanel->clear();
    outputPanel->appendPlainText("▶ Compiling: " + filePath + "\n");

    QString outFile = QFileInfo(filePath).absolutePath() + "/" +
                      QFileInfo(filePath).baseName();
#ifdef Q_OS_WIN
    outFile += ".exe";
#endif

    QStringList args = { filePath, "-o", outFile, "-std=c++17", "-Wall" };
    compileProcess->disconnect();

    connect(compileProcess,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, outFile](int code, QProcess::ExitStatus) {
        if (code == 0) {
            outputPanel->appendPlainText("\n✅ Compiled! Running...\n─────────────────");
            QProcess *run = new QProcess(this);
            run->setProcessChannelMode(QProcess::MergedChannels);
            connect(run, &QProcess::readyRead, this, [this, run](){
                outputPanel->appendPlainText(run->readAll());
            });
            connect(run, QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),
                    this, [this](int exitCode, QProcess::ExitStatus){
                outputPanel->appendPlainText(
                    QString("\n─────────────────\n✔ Exited with code %1").arg(exitCode));
            });
            run->start(outFile, {});
        } else {
            outputPanel->appendPlainText("\n❌ Compilation failed.");
        }
    });

    connect(compileProcess, &QProcess::readyReadStandardError, this, [this](){
        outputPanel->appendPlainText(compileProcess->readAllStandardError());
    });

    compileProcess->start("g++", args);
}

void Editor::newFile() { newTab(); }

void Editor::openFile() {
    QString path = QFileDialog::getOpenFileName(this, "Open File", {},
        "C++ Files (*.cpp *.h *.hpp *.c);;All Files (*)");
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open:\n" + path);
        return;
    }
    QTextStream in(&f);
    newTab();
    CodeEditor *ed = currentEditor();
    ed->setPlainText(in.readAll());
    ed->filePath = path;
    ed->document()->setModified(false);
    tabWidget->setTabText(tabWidget->currentIndex(), QFileInfo(path).fileName());
    setCurrentFile(path);
}

bool Editor::saveFile() {
    CodeEditor *ed = currentEditor();
    if (!ed) return false;
    if (ed->filePath.isEmpty()) return saveFileAs();
    QFile f(ed->filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save:\n" + ed->filePath);
        return false;
    }
    QTextStream out(&f);
    out << ed->toPlainText();
    ed->document()->setModified(false);
    statusLabel->setText("Saved.");
    return true;
}

bool Editor::saveFileAs() {
    QString path = QFileDialog::getSaveFileName(this, "Save As", {},
        "C++ Files (*.cpp *.h *.hpp);;All Files (*)");
    if (path.isEmpty()) return false;
    CodeEditor *ed = currentEditor();
    if (!ed) return false;
    ed->filePath = path;
    tabWidget->setTabText(tabWidget->currentIndex(), QFileInfo(path).fileName());
    setCurrentFile(path);
    return saveFile();
}

bool Editor::maybeSave() {
    CodeEditor *ed = currentEditor();
    if (!ed || !ed->document()->isModified()) return true;
    auto btn = QMessageBox::warning(this, "Unsaved Changes",
        "Document modified. Save?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (btn == QMessageBox::Save)   return saveFile();
    if (btn == QMessageBox::Cancel) return false;
    return true;
}

void Editor::setCurrentFile(const QString &path) {
    currentFile = path;
    QString shown = path.isEmpty() ? "Untitled" : QFileInfo(path).fileName();
    setWindowTitle(shown + " - Code Editor");
    fileLabel->setText(shown);
}

void Editor::toggleTheme()  { isDarkTheme = !isDarkTheme; applyTheme(); }

void Editor::applyTheme() {
    Theme theme = isDarkTheme ? themeManager->darkTheme() : themeManager->lightTheme();
    QString sheet = isDarkTheme ? themeManager->darkStyleSheet() : themeManager->lightStyleSheet();
    setStyleSheet(sheet);

    // Output panel style
    QString outBg = theme.windowBg.name(), outFg = theme.foreground.name();
    outputPanel->setStyleSheet(QString(
        "QPlainTextEdit{background:%1;color:%2;font-family:'Cascadia Code',monospace;font-size:12px;border:none;}"
    ).arg(outBg, outFg));

    // Apply to all tabs
    for (int i = 0; i < tabWidget->count(); ++i) {
        CodeEditor *ed = qobject_cast<CodeEditor*>(tabWidget->widget(i));
        if (ed) ed->applyTheme(theme);
    }

    statusLabel->setText(isDarkTheme ? "Dark theme" : "Light theme");
}

void Editor::showFindReplace() {
    CodeEditor *ed = currentEditor();
    if (!ed) return;
    // Wrap CodeEditor in a QTextEdit-compatible way via a helper
    // Since FindReplaceDialog expects QTextEdit, we use a workaround
    auto *dlg = new QDialog(this);
    dlg->setWindowTitle("Find & Replace");
    dlg->setMinimumWidth(400);
    dlg->setStyleSheet(styleSheet());
    auto *layout = new QGridLayout(dlg);
    auto *findEdit    = new QLineEdit(dlg);
    auto *replaceEdit = new QLineEdit(dlg);
    auto *caseCb      = new QCheckBox("Case sensitive", dlg);
    auto *wholeCb     = new QCheckBox("Whole word",     dlg);
    auto *statusLbl   = new QLabel("", dlg);
    layout->addWidget(new QLabel("Find:"),    0, 0);
    layout->addWidget(findEdit,               0, 1, 1, 2);
    layout->addWidget(new QLabel("Replace:"), 1, 0);
    layout->addWidget(replaceEdit,            1, 1, 1, 2);
    layout->addWidget(caseCb,  2, 0);
    layout->addWidget(wholeCb, 2, 1);
    auto *btnPrev    = new QPushButton("Prev",        dlg);
    auto *btnNext    = new QPushButton("Next",        dlg);
    auto *btnReplace = new QPushButton("Replace",     dlg);
    auto *btnAll     = new QPushButton("Replace All", dlg);
    auto *btnClose   = new QPushButton("Close",       dlg);
    layout->addWidget(btnPrev,    3, 0);
    layout->addWidget(btnNext,    3, 1);
    layout->addWidget(btnReplace, 3, 2);
    layout->addWidget(btnAll,     4, 0);
    layout->addWidget(btnClose,   4, 2);
    layout->addWidget(statusLbl,  5, 0, 1, 3);

    auto getFlags = [&](bool backward = false) {
        QTextDocument::FindFlags f;
        if (backward)           f |= QTextDocument::FindBackward;
        if (caseCb->isChecked()) f |= QTextDocument::FindCaseSensitively;
        if (wholeCb->isChecked()) f |= QTextDocument::FindWholeWords;
        return f;
    };

    connect(btnNext, &QPushButton::clicked, dlg, [&, ed](){
        bool found = ed->find(findEdit->text(), getFlags());
        statusLbl->setText(found ? "" : "Not found.");
    });
    connect(btnPrev, &QPushButton::clicked, dlg, [&, ed](){
        bool found = ed->find(findEdit->text(), getFlags(true));
        statusLbl->setText(found ? "" : "Not found.");
    });
    connect(btnReplace, &QPushButton::clicked, dlg, [&, ed](){
        QTextCursor c = ed->textCursor();
        if (c.hasSelection()) c.insertText(replaceEdit->text());
        ed->find(findEdit->text(), getFlags());
    });
    connect(btnAll, &QPushButton::clicked, dlg, [&, ed](){
        QTextCursor c = ed->textCursor();
        c.movePosition(QTextCursor::Start);
        ed->setTextCursor(c);
        int count = 0;
        while (ed->find(findEdit->text(), getFlags())) {
            ed->textCursor().insertText(replaceEdit->text());
            ++count;
        }
        statusLbl->setText(count > 0 ?
            QString("Replaced %1 occurrence(s).").arg(count) : "Not found.");
    });
    connect(btnClose, &QPushButton::clicked, dlg, &QDialog::close);
    connect(findEdit, &QLineEdit::returnPressed, btnNext, &QPushButton::click);
    dlg->show();
}

void Editor::changeFont() {
    CodeEditor *ed = currentEditor();
    if (!ed) return;
    bool ok;
    QFont f = QFontDialog::getFont(&ok, ed->font(), this, "Choose Font");
    if (ok) ed->setFont(f);
}

void Editor::changeFontSize(int size) {
    for (int i = 0; i < tabWidget->count(); ++i) {
        CodeEditor *ed = qobject_cast<CodeEditor*>(tabWidget->widget(i));
        if (ed) {
            QFont f = ed->font();
            f.setPointSize(size);
            ed->setFont(f);
        }
    }
}

void Editor::updateStatusBar() {
    CodeEditor *ed = currentEditor();
    if (!ed) return;
    QTextCursor c = ed->textCursor();
    lineColLabel->setText(QString("Ln %1, Col %2")
        .arg(c.blockNumber() + 1).arg(c.columnNumber() + 1));
}

void Editor::documentModified() {
    CodeEditor *ed = currentEditor();
    if (!ed) return;
    bool mod = ed->document()->isModified();
    QString title = windowTitle();
    if (mod  && !title.startsWith("*")) setWindowTitle("*" + title);
    if (!mod && title.startsWith("*"))  setWindowTitle(title.mid(1));
}

void Editor::loadSettings() {
    QSettings s("CodeEditor", "Settings");
    isDarkTheme = s.value("darkTheme", true).toBool();
    resize(s.value("size", QSize(1200, 750)).toSize());
    move(s.value("pos", QPoint(100, 100)).toPoint());
    applyTheme();
}

void Editor::saveSettings() {
    QSettings s("CodeEditor", "Settings");
    s.setValue("darkTheme", isDarkTheme);
    s.setValue("size", size());
    s.setValue("pos",  pos());
}

void Editor::closeEvent(QCloseEvent *event) {
    if (maybeSave()) { saveSettings(); event->accept(); }
    else event->ignore();
}

