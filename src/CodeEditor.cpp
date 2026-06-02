#include "CodeEditor.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    lineNumberArea = new LineNumberArea(this);
    highlighter = new SyntaxHighlighter(document());
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    updateLineNumberAreaWidth(0);
    QFont monoFont("Cascadia Code", 14);
    monoFont.setStyleHint(QFont::Monospace);
    setFont(monoFont);
    setTabStopDistance(28);
    setLineWrapMode(QPlainTextEdit::NoWrap);
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1, max = qMax(1, blockCount());
    while (max >= 10) { max /= 10; ++digits; }
    return 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * (digits + 1);
}

void CodeEditor::updateLineNumberAreaWidth(int) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy) lineNumberArea->scroll(0, dy);
    else    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), currentTheme.lineNumberBg.isValid()
                     ? currentTheme.lineNumberBg : QColor("#181825"));
    QTextBlock block = firstVisibleBlock();
    int blockNumber  = block.blockNumber();
    int top    = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            painter.setPen(currentTheme.lineNumberFg.isValid()
                           ? currentTheme.lineNumberFg : QColor("#585B70"));
            painter.drawText(0, top, lineNumberArea->width()-4,
                             fontMetrics().height(), Qt::AlignRight,
                             QString::number(blockNumber + 1));
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::applyTheme(const Theme &theme) {
    currentTheme = theme;
    highlighter->applyTheme(theme);
    setStyleSheet(QString(
        "QPlainTextEdit{background-color:%1;color:%2;border:none;"
        "selection-background-color:%3;"
        "font-family:'Cascadia Code','Consolas',monospace;}"
    ).arg(theme.background.name(), theme.foreground.name(), theme.selectionBg.name()));
    update();
}
