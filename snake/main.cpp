#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("贪吃蛇");
    w.setWindowFlags(w.windowFlags() &~ Qt::WindowMaximizeButtonHint);
    w.statusBar()->setSizeGripEnabled(false);
    w.setFixedSize(800, 600);
    w.show();

    return a.exec();
}
