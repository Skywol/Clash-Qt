//
// Created by cyril on 2020/8/19.
//

#include <QDebug>
#include "mainwindow.h"
#include "proxypage.h"

constexpr int genralBtnId = 1;
constexpr int proxyBtnId = 2;
constexpr int logBtnId = 3;
constexpr int conenctionBtnId = 4;
constexpr int profileBtnId = 5;
constexpr int settingBtnId = 6;

QString toNetSpeedString(int speed){
    const static QVector<const char*> uint = {"B/s", "KB/s", "MB/s", "GB/s"};
    float fSpeed = speed;
    uint8_t count = 0;
    while (count < 3 && fSpeed >= 1024){
        fSpeed/=1024;
        count++;
    }
    return QString::number(fSpeed,'f',2) + " " + uint[count];
}

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent), restfulApi(Clash::RestfulApi::getInstance()){
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName());
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

    connect(&restfulApi, &Clash::RestfulApi::trafficUpdate, this, [this](int up, int down){
        ui->up->setText(toNetSpeedString(up));
        ui->down->setText(toNetSpeedString(down));
    });

    ui->conetentArea->addWidget(new ProxyPage);
}

void MainWindow::onButtonToggled(QAbstractButton *button, bool checked) {
    if(!checked) return;
    if(currentButton != nullptr){
        currentButton->setChecked(false);
    }
    currentButton = button;
    qDebug()<<button->text()<<" "<<checked<<"\n";
}
