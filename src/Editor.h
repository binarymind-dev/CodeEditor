#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QDockWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QLabel>
#include <QComboBox>
#include <QFont>
#include <QFontDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>
#include <QSettings>
#include <QWidget>
#include <QTabWidget>
#include <QProcess>
#include <QTimer>

#include "SyntaxHighlighter.h"
#include "FindReplaceDialog.h"
#include "CodeEditor.h"
#include "ThemeManager.h"

#include "CodeEditor.h"

class Editor : public QMainWindow {
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);
    ~Editor();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    void toggleTheme();
    void showFindReplace();
    void changeFont();
    void updateStatusBar();
    void documentModified();
    void changeFontSize(int size);
    void runCode();
    void newTab();
    void closeTab(int index);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupOutputPanel();
    void applyTheme();
    void loadSettings();
    void saveSettings();
    bool maybeSave();
    void setCurrentFile(const QString &filePath);
    CodeEditor* currentEditor();

    QTabWidget      *tabWidget;
    QLabel          *statusLabel;
    QLabel          *lineColLabel;
    QLabel          *fileLabel;
    QComboBox       *fontSizeBox;
    QToolBar        *toolBar;
    ThemeManager    *themeManager;
    QDockWidget     *outputDock;
    QPlainTextEdit  *outputPanel;
    QProcess        *compileProcess;
    QTimer          *autoSaveTimer;

    QString currentFile;
    bool isDarkTheme;
    bool isModified;
};
