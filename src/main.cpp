#include <QApplication>
#include "Editor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Code Editor");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CodeEditor");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    Editor editor;
    editor.show();
    return app.exec();
}
