#ifndef CLASH_QT_TRAY_H
#define CLASH_QT_TRAY_H

#include <QSystemTrayIcon>

class Tray : public QSystemTrayIcon {
public:
    Tray(QObject *parent = nullptr);

public slots:
    void onActivate(QSystemTrayIcon::ActivationReason reason);

private:
    QMenu *menu;
};


#endif  // CLASH_QT_TRAY_H
