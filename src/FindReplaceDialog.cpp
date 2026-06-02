#include "FindReplaceDialog.h"
#include <QTextCursor>

FindReplaceDialog::FindReplaceDialog(QTextEdit *editor, QWidget *parent)
    : QDialog(parent), textEdit(editor)
{
    setWindowTitle("Find & Replace");
    setMinimumWidth(400);
    auto *layout = new QGridLayout(this);
    layout->addWidget(new QLabel("Find:"),    0, 0);
    findEdit = new QLineEdit(this);
    layout->addWidget(findEdit,               0, 1, 1, 2);
    layout->addWidget(new QLabel("Replace:"), 1, 0);
    replaceEdit = new QLineEdit(this);
    layout->addWidget(replaceEdit,            1, 1, 1, 2);
    caseSensitive = new QCheckBox("Case sensitive", this);
    wholeWord     = new QCheckBox("Whole word",     this);
    layout->addWidget(caseSensitive, 2, 0);
    layout->addWidget(wholeWord,     2, 1);
    auto *btnPrev    = new QPushButton("Prev",        this);
    auto *btnNext    = new QPushButton("Next",        this);
    auto *btnReplace = new QPushButton("Replace",     this);
    auto *btnAll     = new QPushButton("Replace All", this);
    auto *btnClose   = new QPushButton("Close",       this);
    layout->addWidget(btnPrev,    3, 0);
    layout->addWidget(btnNext,    3, 1);
    layout->addWidget(btnReplace, 3, 2);
    layout->addWidget(btnAll,     4, 0);
    layout->addWidget(btnClose,   4, 2);
    statusLabel = new QLabel("", this);
    statusLabel->setStyleSheet("color:gray;font-size:12px;");
    layout->addWidget(statusLabel, 5, 0, 1, 3);
    connect(btnNext,    &QPushButton::clicked, this, &FindReplaceDialog::findNext);
    connect(btnPrev,    &QPushButton::clicked, this, &FindReplaceDialog::findPrev);
    connect(btnReplace, &QPushButton::clicked, this, &FindReplaceDialog::replaceOne);
    connect(btnAll,     &QPushButton::clicked, this, &FindReplaceDialog::replaceAll);
    connect(btnClose,   &QPushButton::clicked, this, &QDialog::close);
    connect(findEdit,   &QLineEdit::returnPressed, this, &FindReplaceDialog::findNext);
}

void FindReplaceDialog::findNext() {
    if (findEdit->text().isEmpty()) return;
    QTextDocument::FindFlags f;
    if (caseSensitive->isChecked()) f |= QTextDocument::FindCaseSensitively;
    if (wholeWord->isChecked())     f |= QTextDocument::FindWholeWords;
    bool found = textEdit->find(findEdit->text(), f);
    statusLabel->setText(found ? "" : "Not found.");
}

void FindReplaceDialog::findPrev() {
    if (findEdit->text().isEmpty()) return;
    QTextDocument::FindFlags f = QTextDocument::FindBackward;
    if (caseSensitive->isChecked()) f |= QTextDocument::FindCaseSensitively;
    if (wholeWord->isChecked())     f |= QTextDocument::FindWholeWords;
    bool found = textEdit->find(findEdit->text(), f);
    statusLabel->setText(found ? "" : "Not found.");
}

void FindReplaceDialog::replaceOne() {
    QTextCursor c = textEdit->textCursor();
    if (c.hasSelection() && c.selectedText() == findEdit->text())
        c.insertText(replaceEdit->text());
    findNext();
}

void FindReplaceDialog::replaceAll() {
    if (findEdit->text().isEmpty()) return;
    QTextCursor c = textEdit->textCursor();
    c.movePosition(QTextCursor::Start);
    textEdit->setTextCursor(c);
    QTextDocument::FindFlags f;
    if (caseSensitive->isChecked()) f |= QTextDocument::FindCaseSensitively;
    if (wholeWord->isChecked())     f |= QTextDocument::FindWholeWords;
    int count = 0;
    while (textEdit->find(findEdit->text(), f)) {
        textEdit->textCursor().insertText(replaceEdit->text());
        ++count;
    }
    statusLabel->setText(count > 0 ? QString("Replaced %1 occurrence(s).").arg(count) : "Not found.");
}
