//
// Created by cyril on 2020/8/19.
//

#include <QDebug>
#include "mainwindow.h"

constexpr int genralBtnId = 1;
constexpr int proxyBtnId = 2;
constexpr int logBtnId = 3;
constexpr int conenctionBtnId = 4;
constexpr int profileBtnId = 5;
constexpr int settingBtnId = 6;


MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent){
    ui = new Ui::MainWindow;
    ui->setupUi(this);

    //Menu Tab Button
    group = new QButtonGroup(this);
    group->addButton(ui->genralBtn, genralBtnId);
    group->addButton(ui->proxyBtn, proxyBtnId);
    group->addButton(ui->logBtn, logBtnId);
    group->addButton(ui->connectionBtn, conenctionBtnId);
    group->addButton(ui->profileBtn, profileBtnId);
    group->addButton(ui->settingBtn, settingBtnId);

    currentButton = nullptr;
    connect(group, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled), this, &MainWindow::onButtonToggled);
}

void MainWindow::onButtonToggled(QAbstractButton *button, bool checked) {
    if(!checked) return;
    if(currentButton != nullptr){
        currentButton->setChecked(false);
    }
    currentButton = button;
    qDebug()<<button->text()<<" "<<checked<<"\n";
}
