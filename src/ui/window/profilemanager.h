#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QDialog>

#include "clash/clash.h"

namespace Ui {
class ProfileManager;
}

class ProfileTableModel;

class ProfileManager : public QDialog {
    Q_OBJECT

public:
    explicit ProfileManager(Clash::ProfileList &profiles, QWidget *parent = nullptr);

    void downloadProfile();
    void downloadFinished(QNetworkReply *reply);

private:
    Ui::ProfileManager *ui;
    Clash::ProfileList &profiles;
    ProfileTableModel *model;
    QNetworkAccessManager *manager;
};

#endif  // PROFILEMANAGER_H
