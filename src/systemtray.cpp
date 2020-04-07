#include "systemtray.h"
#include "subscribemanager.h"
#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QElapsedTimer>
#include <QDesktopServices>
#include <QUrl>

SystemTray::SystemTray(){


    setIcon(QIcon(":/icon/clash.png"));
    menu = new QMenu();
    initMenu();
    connect(this, &QSystemTrayIcon::activated, this, &SystemTray::onTrayClicked);

    subscribe = new SubscribeManager();
    connect(subscribe, &SubscribeManager::updateFinish, this, [this](int suc, int err){
        this->showMessage(tr("Update Finished"), tr("%1 succeed, %2 failed").arg(suc).arg(err));
        emit needRestartClash();
    });
}

void SystemTray::onTrayClicked(QSystemTrayIcon::ActivationReason reason) {
    switch(reason){
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            emit showWebuiClicked();
            break;
        case QSystemTrayIcon::MiddleClick:
            copyCommand();
            break;
        case QSystemTrayIcon::Context:
            contextMenu()->show();
        default:
            break;
    }
}

void SystemTray::initMenu() {
    setContextMenu(menu);

    auto *showWindow = new QAction(tr("Show Clash-Qt"));
    connect(showWindow, &QAction::triggered, this, [this]{emit showWebuiClicked();});
    menu->addAction(showWindow);
    menu->addSeparator();

    auto *restartClashAction = new QAction(tr("Restart Clash Core"));
    connect(restartClashAction, &QAction::triggered, this, [this]{
        emit needRestartClash();
    });
    menu->addAction(restartClashAction);

    auto *subscribeMenu = new QMenu(tr("Subscribe"));
    auto *mangeSubscribeAction = new QAction(tr("Manage"));
    connect(mangeSubscribeAction, &QAction::triggered, this, [this]{subscribe->show();subscribe->setFocus();});
    auto *updateSubscribeAction = new QAction(tr("Update"));
    connect(updateSubscribeAction, &QAction::triggered, this, [this]{subscribe->updateSubscribe();});
    subscribeMenu->addAction(mangeSubscribeAction);
    subscribeMenu->addAction(updateSubscribeAction);
    menu->addMenu(subscribeMenu);

    auto *copyCommandAction = new QAction(tr("Copy Proxy Command"));
    connect(copyCommandAction, &QAction::triggered, this, &SystemTray::copyCommand);
    menu->addAction(copyCommandAction);

    auto *helpMenu = new QMenu(tr("Help"));
    auto *clash_log_action = new QAction(tr("Open Clash Core Log folder"));
    connect(clash_log_action, &QAction::triggered, this, [this]{
        QString dir_name = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/clash_log";
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir_name));
    });
    helpMenu->addAction(clash_log_action);
    menu->addMenu(helpMenu);

    menu->addSeparator();
    auto *quitAction = new QAction(tr("Quit"));
    connect(quitAction, &QAction::triggered, this, [this]{QApplication::quit();});
    menu->addAction(quitAction);
}

void SystemTray::copyCommand() {
    QString command = QString("export HTTP_PROXY=http://127.0.0.1:%1;export HTTPS_PROXY=http://127.0.0.1:%1;export ALL_PROXY=http://127.0.0.1:%1;")
            .arg(ClashConfig::http_port);
    QApplication::clipboard()->setText(command);
    qDebug()<<QApplication::clipboard()->text();
}

SystemTray::~SystemTray() {
    delete subscribe;
}


