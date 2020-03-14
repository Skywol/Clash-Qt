#include "systemtray.h"
#include "subscribemanager.h"
#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QTextEdit>
#include <QClipboard>
#include <QElapsedTimer>

SystemTray::SystemTray(){
#ifndef DEBUG
    clash.start();
#endif
    setIcon(QIcon(":/icon/clash.png"));
    menu = new QMenu();
    initMenu();
    connect(this, &QSystemTrayIcon::activated, this, &SystemTray::onTrayClicked);

    //Clash-Core Log
    clash_output = new QTextEdit();
    clash_output->setWindowTitle(tr("Clash Core Log"));
    clash_output->resize(600, 700);
    connect(&clash, &Clash::readyRead, this, [this](QByteArray data){clash_output->append(data);});

    connect(&clash, &Clash::clashStarted, this, [this]{
        QElapsedTimer t;
        t.start();
        // wait 1 sec to wait clash finish starting.
        while(t.elapsed()<1000){
            QCoreApplication::processEvents(); // Handle event when in loop
        }
        w.reload();
    });
    subscribe = new SubscribeManager();
    connect(subscribe, &SubscribeManager::updateFinish, this, [this](int suc, int err){
        this->showMessage(tr("Update Finished"), tr("%1 succeed, %2 failed").arg(suc).arg(err));
        clash.restart();
    });
}

void SystemTray::onTrayClicked(QSystemTrayIcon::ActivationReason reason) {
    switch(reason){
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            w.show();
            w.setFocus();
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
    connect(showWindow, &QAction::triggered, this, [this]{this->w.show();});
    menu->addAction(showWindow);
    menu->addSeparator();

    auto *restartClashAction = new QAction(tr("Restart Clash Core"));
    connect(restartClashAction, &QAction::triggered, this, [this]{
        clash.restart();
        w.reload();
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
    auto *clash_log_action = new QAction(tr("Show Clash Core Log"));
    connect(clash_log_action, &QAction::triggered, this, [this]{clash_output->show();});
    helpMenu->addAction(clash_log_action);
    menu->addMenu(helpMenu);

    menu->addSeparator();
    auto *quitAction = new QAction(tr("Quit"));
    connect(quitAction, &QAction::triggered, this, [this]{clash.stop();exit(0);});
    menu->addAction(quitAction);
}

void SystemTray::copyCommand() {
    QString command = QString("export https_proxy=http://127.0.0.1:%1;export http_proxy=http://127.0.0.1:%1;export all_proxy=socks5://127.0.0.1:%2;")
            .arg(ClashConfig::http_port).arg(ClashConfig::socks_port);
    QApplication::clipboard()->setText(command);
    qDebug()<<QApplication::clipboard()->text();
}

SystemTray::~SystemTray() {
    delete subscribe;
}
