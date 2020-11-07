#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <csignal>

#include "clash/clash.h"
#include "ui/window/mainwindow.h"
#include "ui/window/tray.h"
#include "util/instance.h"

void signalhandler(int sig) {
    if (sig == SIGINT) {
        qApp->quit();
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    signal(SIGINT, signalhandler);

    a.setApplicationName("Clash-Qt");
    a.setWindowIcon(QIcon(":/icon/clash.png"));
    a.setQuitOnLastWindowClosed(false);

    QFile styleFile(":/style.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString style = styleFile.readAll();
    a.setStyleSheet(style);

    getInstance<MainWindow>();
    getInstance<Tray>().show();
    getInstance<Clash>().start();

    return a.exec();
}