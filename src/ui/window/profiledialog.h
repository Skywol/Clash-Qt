#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QDialog>

#include "clash/clash.h"

namespace Ui {
class ProfileDialog;
}

class ProfileTableModel;

class ProfileDialog : public QDialog {
    Q_OBJECT

public:
    explicit ProfileDialog(Clash::ProfileList &profiles, QWidget *parent = nullptr);
    ~ProfileDialog() override;

    void downloadProfile();
    void downloadFinished(QNetworkReply *reply);

private:
    Ui::ProfileDialog *ui;
    Clash::ProfileList &profiles;
    ProfileTableModel *model;
    QNetworkAccessManager *manager;
};

#endif  // PROFILEMANAGER_H
