#include <QApplication>
#include <QDir>
#include <QStandardPaths>

#include "clashconfig.h"
#include "systemtray.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    a.setApplicationName("Clash-Qt");
    a.setWindowIcon(QIcon(":/icon/clash.png"));
    a.setQuitOnLastWindowClosed(false);

    if(not QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)).exists()){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }

    ClashConfig::combineConfig();

    SystemTray tray;
    tray.show();

    return a.exec();
}
