
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QFontDatabase>
#include <QFile>

void AddCustomFonts()
{
    QFile res(":/new/prefix1/tahoma.ttf");
    if (res.open(QIODevice::ReadOnly))
    {
        QFontDatabase::addApplicationFontFromData(res.readAll());
    }
}

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

    AddCustomFonts();

    MainWindow w;
    w.show();

    return a.exec();
}

