#include "ThemeManager.h"

ThemeManager::ThemeManager() {}
Theme ThemeManager::darkTheme()  const { return buildDark(); }
Theme ThemeManager::lightTheme() const { return buildLight(); }

Theme ThemeManager::buildDark() const {
    Theme t;
    t.name = "Dark";
    t.background    = QColor("#1E1E2E"); t.foreground   = QColor("#CDD6F4");
    t.lineNumberBg  = QColor("#181825"); t.lineNumberFg  = QColor("#585B70");
    t.selectionBg   = QColor("#313244"); t.currentLineBg = QColor("#252535");
    t.cursorColor   = QColor("#F5C2E7");
    t.keyword       = QColor("#CBA6F7"); t.string        = QColor("#A6E3A1");
    t.comment       = QColor("#6C7086"); t.number        = QColor("#FAB387");
    t.preprocessor  = QColor("#89DCEB"); t.function      = QColor("#89B4FA");
    t.type          = QColor("#F38BA8");
    t.windowBg      = QColor("#11111B"); t.menuBg        = QColor("#1E1E2E");
    t.menuFg        = QColor("#CDD6F4"); t.toolbarBg     = QColor("#181825");
    t.statusBg      = QColor("#181825"); t.statusFg      = QColor("#A6ADC8");
    t.borderColor   = QColor("#313244");
    return t;
}

Theme ThemeManager::buildLight() const {
    Theme t;
    t.name = "Light";
    t.background    = QColor("#FFFFFF"); t.foreground    = QColor("#24273A");
    t.lineNumberBg  = QColor("#F4F4F5"); t.lineNumberFg  = QColor("#9CA0B0");
    t.selectionBg   = QColor("#DCE0F5"); t.currentLineBg = QColor("#F5F5F9");
    t.cursorColor   = QColor("#7287FD");
    t.keyword       = QColor("#7287FD"); t.string        = QColor("#40A02B");
    t.comment       = QColor("#9CA0B0"); t.number        = QColor("#FE640B");
    t.preprocessor  = QColor("#179299"); t.function      = QColor("#1E66F5");
    t.type          = QColor("#D20F39");
    t.windowBg      = QColor("#EFF1F5"); t.menuBg        = QColor("#FFFFFF");
    t.menuFg        = QColor("#4C4F69"); t.toolbarBg     = QColor("#F4F4F5");
    t.statusBg      = QColor("#E6E9EF"); t.statusFg      = QColor("#4C4F69");
    t.borderColor   = QColor("#CAC9D4");
    return t;
}

QString ThemeManager::darkStyleSheet() const {
    Theme t = buildDark();
    return QString(
        "QMainWindow,QWidget{background-color:%1;color:%2;font-family:'Segoe UI',Arial,sans-serif;}"
        "QMenuBar{background-color:%3;color:%2;border-bottom:1px solid %4;padding:2px;}"
        "QMenuBar::item:selected{background-color:%5;border-radius:4px;}"
        "QMenu{background-color:%3;color:%2;border:1px solid %4;border-radius:6px;padding:4px;}"
        "QMenu::item:selected{background-color:%5;border-radius:4px;}"
        "QToolBar{background-color:%6;border-bottom:1px solid %4;spacing:4px;padding:4px;}"
        "QToolButton{background:transparent;border:1px solid transparent;border-radius:5px;padding:4px 8px;color:%2;}"
        "QToolButton:hover{background-color:%5;border-color:%4;}"
        "QStatusBar{background-color:%6;color:%7;border-top:1px solid %4;font-size:12px;}"
        "QComboBox{background-color:%5;color:%2;border:1px solid %4;border-radius:5px;padding:3px 8px;}"
        "QScrollBar:vertical{background:%1;width:10px;border-radius:5px;}"
        "QScrollBar::handle:vertical{background:%4;border-radius:5px;min-height:20px;}"
        "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0;}"
        "QTextEdit{background-color:%8;color:%2;border:none;selection-background-color:%5;"
        "font-family:'Cascadia Code','Fira Code','Consolas',monospace;font-size:14px;}"
    ).arg(t.windowBg.name(), t.menuFg.name(), t.menuBg.name(), t.borderColor.name(),
          t.lineNumberBg.name(), t.toolbarBg.name(), t.statusFg.name(), t.background.name());
}

QString ThemeManager::lightStyleSheet() const {
    Theme t = buildLight();
    return QString(
        "QMainWindow,QWidget{background-color:%1;color:%2;font-family:'Segoe UI',Arial,sans-serif;}"
        "QMenuBar{background-color:%3;color:%2;border-bottom:1px solid %4;padding:2px;}"
        "QMenuBar::item:selected{background-color:%5;border-radius:4px;}"
        "QMenu{background-color:%3;color:%2;border:1px solid %4;border-radius:6px;padding:4px;}"
        "QMenu::item:selected{background-color:%5;border-radius:4px;}"
        "QToolBar{background-color:%6;border-bottom:1px solid %4;spacing:4px;padding:4px;}"
        "QToolButton{background:transparent;border:1px solid transparent;border-radius:5px;padding:4px 8px;color:%2;}"
        "QToolButton:hover{background-color:%4;}"
        "QStatusBar{background-color:%6;color:%7;border-top:1px solid %4;font-size:12px;}"
        "QComboBox{background-color:%3;color:%2;border:1px solid %4;border-radius:5px;padding:3px 8px;}"
        "QScrollBar:vertical{background:%1;width:10px;border-radius:5px;}"
        "QScrollBar::handle:vertical{background:%4;border-radius:5px;min-height:20px;}"
        "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0;}"
        "QTextEdit{background-color:%8;color:%2;border:none;selection-background-color:%5;"
        "font-family:'Cascadia Code','Fira Code','Consolas',monospace;font-size:14px;}"
    ).arg(t.windowBg.name(), t.menuFg.name(), t.menuBg.name(), t.borderColor.name(),
          t.selectionBg.name(), t.toolbarBg.name(), t.statusFg.name(), t.background.name());
}
