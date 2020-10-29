#include <QApplication>
#include <QDir>
#include <QStandardPaths>

#include "ui/window/mainwindow.h"
#include "util/instance.h"
int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
#ifdef DEBUG
    a.setApplicationName("Clash-Qt-Debug");
#else
    a.setApplicationName("Clash-Qt");
#endif
    a.setWindowIcon(QIcon(":/icon/clash.png"));
    //a.setQuitOnLastWindowClosed(false);

    QFile styleFile(":/style.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString style = styleFile.readAll();
    a.setStyleSheet(style);

    if(! QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)).exists()){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }
    if(! QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/clash_log").exists()){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/clash_log");
    }

    getInstance<MainWindow>().show();

    return a.exec();
}