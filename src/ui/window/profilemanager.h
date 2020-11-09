#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QDialog>

#include "clash/clash.h"

namespace Ui {
class ProfileManager;
}

class ProfileManager : public QDialog {
    Q_OBJECT

public:
    explicit ProfileManager(QList<Clash::Profile> &profiles, QWidget *parent = nullptr);
    ~ProfileManager();

private:
    Ui::ProfileManager *ui;
};

#endif  // PROFILEMANAGER_H
