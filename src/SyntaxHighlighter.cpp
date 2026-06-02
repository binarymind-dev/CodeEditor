#include "SyntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    ThemeManager mgr;
    applyTheme(mgr.darkTheme());
}

void SyntaxHighlighter::applyTheme(const Theme &theme) {
    buildRules(theme);
    rehighlight();
}

void SyntaxHighlighter::buildRules(const Theme &theme) {
    rules.clear();
    HighlightRule rule;

    QTextCharFormat kwFmt;
    kwFmt.setForeground(theme.keyword);
    kwFmt.setFontWeight(QFont::Bold);
    const QStringList keywords = {
        "\\bauto\\b","\\bbool\\b","\\bbreak\\b","\\bcase\\b","\\bcatch\\b",
        "\\bchar\\b","\\bclass\\b","\\bconst\\b","\\bconstexpr\\b","\\bcontinue\\b",
        "\\bdefault\\b","\\bdelete\\b","\\bdo\\b","\\bdouble\\b","\\belse\\b",
        "\\benum\\b","\\bexplicit\\b","\\bextern\\b","\\bfalse\\b","\\bfloat\\b",
        "\\bfor\\b","\\bfriend\\b","\\bgoto\\b","\\bif\\b","\\binline\\b",
        "\\bint\\b","\\blong\\b","\\bmutable\\b","\\bnamespace\\b","\\bnew\\b",
        "\\bnoexcept\\b","\\bnullptr\\b","\\boperator\\b","\\bprivate\\b",
        "\\bprotected\\b","\\bpublic\\b","\\breturn\\b","\\bshort\\b",
        "\\bsigned\\b","\\bsizeof\\b","\\bstatic\\b","\\bstruct\\b",
        "\\bswitch\\b","\\btemplate\\b","\\bthis\\b","\\bthrow\\b",
        "\\btrue\\b","\\btry\\b","\\btypedef\\b","\\btypename\\b",
        "\\bunion\\b","\\bunsigned\\b","\\busing\\b","\\bvirtual\\b",
        "\\bvoid\\b","\\bvolatile\\b","\\bwhile\\b"
    };
    for (const QString &kw : keywords) {
        rule.pattern = QRegularExpression(kw);
        rule.format  = kwFmt;
        rules.append(rule);
    }

    QTextCharFormat ppFmt;
    ppFmt.setForeground(theme.preprocessor);
    rule.pattern = QRegularExpression("^\\s*#\\s*\\w+");
    rule.format  = ppFmt;
    rules.append(rule);

    QTextCharFormat strFmt;
    strFmt.setForeground(theme.string);
    rule.pattern = QRegularExpression("\"(?:[^\"\\\\]|\\\\.)*\"");
    rule.format  = strFmt;
    rules.append(rule);
    rule.pattern = QRegularExpression("'(?:[^'\\\\]|\\\\.)*'");
    rules.append(rule);

    QTextCharFormat numFmt;
    numFmt.setForeground(theme.number);
    rule.pattern = QRegularExpression("\\b(0x[0-9A-Fa-f]+|\\d+\\.?\\d*)\\b");
    rule.format  = numFmt;
    rules.append(rule);

    QTextCharFormat fnFmt;
    fnFmt.setForeground(theme.function);
    rule.pattern = QRegularExpression("\\b([a-zA-Z_][a-zA-Z0-9_]*)\\s*(?=\\()");
    rule.format  = fnFmt;
    rules.append(rule);

    QTextCharFormat cmtFmt;
    cmtFmt.setForeground(theme.comment);
    cmtFmt.setFontItalic(true);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format  = cmtFmt;
    rules.append(rule);

    multiLineCommentFormat = cmtFmt;
    commentStart = QRegularExpression("/\\*");
    commentEnd   = QRegularExpression("\\*/");
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightRule &rule : rules) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch m = it.next();
            setFormat(m.capturedStart(), m.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int startIdx = 0;
    if (previousBlockState() != 1)
        startIdx = text.indexOf(commentStart);
    while (startIdx >= 0) {
        QRegularExpressionMatch endMatch = commentEnd.match(text, startIdx);
        int commentLen;
        if (!endMatch.hasMatch()) {
            setCurrentBlockState(1);
            commentLen = text.length() - startIdx;
        } else {
            commentLen = endMatch.capturedStart() - startIdx + endMatch.capturedLength();
        }
        setFormat(startIdx, commentLen, multiLineCommentFormat);
        startIdx = text.indexOf(commentStart, startIdx + commentLen);
    }
}
