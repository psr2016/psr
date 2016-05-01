/*
 * main.cpp
 */

#include <QGuiApplication>
#include <QApplication>
#include <QFileInfo>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set OpenGL Version information
    // Note: This format must be set before show() is called.
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    if (!QFileInfo(argv[1]).isReadable()) {
        printf("Error! INI file not present\n");
        exit(1);
    }


    // Set the window up
    MainWindow window(new QSettings(argv[1], QSettings::IniFormat));
    window.setFormat(format);
    window.resize(QSize(500, 400));
    window.show();
    window.openPlot();

    return app.exec();
}

