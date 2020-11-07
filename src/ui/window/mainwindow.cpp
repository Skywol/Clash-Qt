#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QtWidgets/QStatusBar>

#include "connectionpage.h"
#include "logpage.h"
#include "profilepage.h"
#include "proxypage.h"
#include "ui/widget/netspeedlabel.h"
#include "util/instance.h"

MainWindow::MainWindow(QWidget *parent) : clash(::getInstance<Clash>()) {
    setWindowTitle(QApplication::applicationName());
    resize(800, 600);
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    tabWidget->addTab(new ProxyPage(this), tr("Proxy"));
    tabWidget->addTab(new LogPage(this), tr("Log"));
    tabWidget->addTab(new ConnectionPage, tr("Connection"));
    tabWidget->addTab(new ProfilePage, tr("Profile"));

    QStatusBar *bar = statusBar();
    netSpeedLabel = new NetSpeedLabel(this);
    bar->addPermanentWidget(netSpeedLabel);
    connect(clash.api(), &Clash::RestfulApi::trafficUpdate, netSpeedLabel, &NetSpeedLabel::setSpeed);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChange);
}

void MainWindow::onTabChange(int index) {}
