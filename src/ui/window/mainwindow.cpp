#include "mainwindow.h"
#include "proxypage.h"
#include "logpage.h"
#include "connectionpage.h"
#include "ui/widget/netspeedlabel.h"
#include "util/instance.h"
#include <QApplication>
#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QtWidgets/QStatusBar>

MainWindow::MainWindow(QWidget *parent):
restfulApi(::getInstance<Clash::RestfulApi>()){
    setWindowTitle(QApplication::applicationName());
    resize(800, 600);
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    tabWidget->addTab(new ProxyPage(this), "Proxy");
    tabWidget->addTab(new LogPage(this), "Log");
    tabWidget->addTab(new ConnectionPage, "Connection");

    QStatusBar *bar = statusBar();
    netSpeedLabel = new NetSpeedLabel(this);
    bar->addPermanentWidget(netSpeedLabel);
    connect(&restfulApi, &Clash::RestfulApi::trafficUpdate, netSpeedLabel, &NetSpeedLabel::setSpeed);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChange);

    restfulApi.listenTraffic();
    restfulApi.listenLog();

    QTimer *timer = new QTimer;
    timer->setInterval(1000);
    timer->start();
    restfulApi.autoUpdateProxy(true, 1000);
    restfulApi.autoUpdateConnection(true, 2000);
}

void MainWindow::onTabChange(int index) {

}
