#ifndef CLASH_QT_SYSTEMTRAY_H
#define CLASH_QT_SYSTEMTRAY_H

#include "mainwindow.h"
#include "clash.h"
#include "clashconfig.h"
#include <QSystemTrayIcon>

class QMenu;
class QTextEdit;
class SubscribeManager;

class SystemTray : public QSystemTrayIcon{
    Q_OBJECT
public:
    SystemTray();
    void onTrayClicked(QSystemTrayIcon::ActivationReason reason);
    void copyCommand();

private:
    inline void initMenu();

    MainWindow w;
    SubscribeManager *subscribe;
    Clash clash;
    QMenu *menu;
    QTextEdit *clash_output;
};


#endif //CLASH_QT_SYSTEMTRAY_H
