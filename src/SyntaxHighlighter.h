#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QVector>
#include "ThemeManager.h"

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QTextDocument *parent = nullptr);
    void applyTheme(const Theme &theme);
protected:
    void highlightBlock(const QString &text) override;
private:
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat    format;
    };
    void buildRules(const Theme &theme);
    QVector<HighlightRule> rules;
    QTextCharFormat        multiLineCommentFormat;
    QRegularExpression     commentStart;
    QRegularExpression     commentEnd;
};
