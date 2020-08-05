#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QtCore/QElapsedTimer>

#include "clashconfig.h"
#include "systemtray.h"
#include "webui.h"

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
    if(! QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/clash_log").exists()){
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/clash_log");
    }

    ClashConfig::combineConfig();
    Clash clash;
    Webui webui;
    SystemTray tray;

    QObject::connect(&clash, &Clash::clashStarted, &webui, [&webui]{
        QElapsedTimer t;
        t.start();
        // wait 1 sec to wait clash finish starting.
        while(t.elapsed()<1000){
            QCoreApplication::processEvents(); // Handle event when in loop
        }
        Clash::load();
        webui.reload();
    });
    QObject::connect(&tray, &SystemTray::needRestartClash, &clash, &Clash::restart);
    QObject::connect(&tray, &SystemTray::showWebuiClicked, &webui, [&webui]{
        webui.show();
        webui.setFocus(Qt::PopupFocusReason);
    });
    QObject::connect(&a, &QApplication::aboutToQuit, &clash, &Clash::stop);

    tray.show();

    clash.start();

    return a.exec();
}
