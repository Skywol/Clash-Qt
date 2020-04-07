#ifndef CLASH_QT_SYSTEMTRAY_H
#define CLASH_QT_SYSTEMTRAY_H

#include "clash.h"
#include "clashconfig.h"
#include <QSystemTrayIcon>
#include <QThread>

class QMenu;
class QTextEdit;
class SubscribeManager;

class SystemTray : public QSystemTrayIcon{
    Q_OBJECT
signals:
    void showWebuiClicked();
    void needRestartClash();
public:
    SystemTray();
    ~SystemTray() override;
    void onTrayClicked(QSystemTrayIcon::ActivationReason reason);
    void copyCommand();
    void appendClashOutput(QByteArray o);

private:
    inline void initMenu();

    SubscribeManager *subscribe;
    QMenu *menu;
};

#endif //CLASH_QT_SYSTEMTRAY_H
