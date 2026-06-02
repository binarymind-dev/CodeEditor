#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QTextEdit>

class FindReplaceDialog : public QDialog {
    Q_OBJECT
public:
    explicit FindReplaceDialog(QTextEdit *editor, QWidget *parent = nullptr);
private slots:
    void findNext();
    void findPrev();
    void replaceOne();
    void replaceAll();
private:
    QTextEdit   *textEdit;
    QLineEdit   *findEdit;
    QLineEdit   *replaceEdit;
    QCheckBox   *caseSensitive;
    QCheckBox   *wholeWord;
    QLabel      *statusLabel;
};
