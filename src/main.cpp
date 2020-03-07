#include <QApplication>
#include <QDir>
#include <QStandardPaths>

#include "clashconfig.h"
#include "systemtray.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
#ifdef DEBUG
    a.setApplicationName("Clash-Qt-Debug");
#else
    a.setApplicationName("Clash-Qt");
#endif
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
