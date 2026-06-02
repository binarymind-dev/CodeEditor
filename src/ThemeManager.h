#pragma once
#include <QString>
#include <QColor>
#include <QFont>

struct Theme {
    QString name;
    QColor background, foreground, lineNumberBg, lineNumberFg;
    QColor selectionBg, currentLineBg, cursorColor;
    QColor keyword, string, comment, number, preprocessor, function, type;
    QColor windowBg, menuBg, menuFg, toolbarBg, statusBg, statusFg, borderColor;
};

class ThemeManager {
public:
    ThemeManager();
    Theme darkTheme()  const;
    Theme lightTheme() const;
    QString darkStyleSheet()  const;
    QString lightStyleSheet() const;
private:
    Theme buildDark()  const;
    Theme buildLight() const;
};
