#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-static-accessed-through-instance"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>

#include "ui/window/mainwindow.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
#ifdef DEBUG
    a.setApplicationName("Clash-Qt-Debug");
#else
    a.setApplicationName("Clash-Qt");
#endif
    a.setWindowIcon(QIcon(":/icon/clash.png"));
    //a.setQuitOnLastWindowClosed(false);

#ifdef _WIN32
    a.setFont(QFont("Microsoft YaHei", 9));
#endif

    QFile styleFile(":/style.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString style = styleFile.readAll();
    a.setStyleSheet(style);

    if(not QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)).exists()){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }
    if(! QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/clash_log").exists()){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/clash_log");
    }

    MainWindow w;
    w.show();

    return a.exec();
}

#pragma clang diagnostic pop