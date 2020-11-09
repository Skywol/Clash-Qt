#include "profilemanager.h"

#include "ui_profilemanager.h"

ProfileManager::ProfileManager(QList<Clash::Profile> &profiles, QWidget *parent) : QDialog(parent), ui(new Ui::ProfileManager) {
    ui->setupUi(this);
}

ProfileManager::~ProfileManager() { delete ui; }
