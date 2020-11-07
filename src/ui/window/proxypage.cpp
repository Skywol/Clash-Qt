#include "proxypage.h"

#include <ui/widget/proxygroupwidget.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QtCore/QFile>

#include "clash/clash.h"
#include "util/instance.h"

ProxyPage::ProxyPage(QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    headerArea = new QHBoxLayout;
    layout->addLayout(headerArea);
    headerArea->setAlignment(Qt::AlignmentFlag::AlignCenter);


    groupArea = new QScrollArea;
    groupLayout = new QVBoxLayout;
    QWidget *inner = new QWidget;
    QVBoxLayout *innerLayout = new QVBoxLayout;
    inner->setLayout(innerLayout);
    innerLayout->addLayout(groupLayout);
    innerLayout->addStretch();
    groupArea->setWidgetResizable(true);
    groupArea->setWidget(inner);
    layout->addWidget(groupArea);
    groupArea->setAlignment(Qt::AlignmentFlag::AlignTop);
    setMode(Clash::RULE);
    connect(getInstance<Clash>().api(), &Clash::RestfulApi::proxyDataReceived, this, &ProxyPage::updateData);

    QFile lastStatus(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/status.yaml");
    if (lastStatus.exists()) {
        lastStatus.open(QIODevice::ReadOnly);
        node = new YAML::Node(YAML::Load(lastStatus.readAll().toStdString()));
        lastStatus.close();
    } else {
        node = new YAML::Node;
    }
}


void ProxyPage::updateData(QByteArray rawJson) {
    QJsonParseError error{};
    QJsonDocument document = QJsonDocument::fromJson(rawJson, &error);
    if (error.error == QJsonParseError::NoError) {
        QJsonObject obj = document.object()["proxies"].toObject();
        if (obj.isEmpty()) {
            qDebug() << "Can not parse data (empty): " << rawJson << "Reason: ";
            return;
        }
        int current_item_number = groupLayout->count();
        if (mode == Clash::RULE) {  // Do not display GLOBAL, DIRECT
            QList<QString> groupList;
            static QSet<QString> hiddenList = {"DIRECT", "GLOBAL"};
            for (auto name : obj.keys()) {
                if (!hiddenList.contains(name)) {
                    groupList.append(name);
                }
            }
            int proxy_index = groupList.indexOf("Proxy");  // record Group Proxy index to display it first;

            // Deal Proxy First
            if (proxy_index >= 0) {
                if (current_item_number == 0) {
                    auto widget = new ProxyGroupWidget(this);
                    widget->updateData(obj["Proxy"].toObject());
                    groupLayout->addWidget(widget);
                } else {
                    auto widget = dynamic_cast<ProxyGroupWidget *>(groupLayout->itemAt(0)->widget());
                    widget->updateData(obj["Proxy"].toObject());
                }
            }
            int totalsize = 0;
            for (int i = 0; i < groupList.count(); i++) {
                auto type = obj[groupList.at(i)].toObject()["type"].toString();
                if (type != "Selector" && type != "URLTest" && type != "Direct" && type != "Reject") {
                    totalsize = i;
                    break;
                }
                if (i == proxy_index) {
                    continue;
                }
                int index = (i < proxy_index) ? i + 1 : i;

                if (index >= current_item_number) {
                    auto widget = new ProxyGroupWidget(this);
                    widget->updateData(obj[groupList.at(i)].toObject());
                    groupLayout->addWidget(widget);
                } else {
                    auto widget = dynamic_cast<ProxyGroupWidget *>(groupLayout->itemAt(index)->widget());
                    widget->updateData(obj[groupList.at(i)].toObject());
                }
            }

            // Delete extra item
            for (int i = totalsize + 1; i < groupLayout->count(); i++) {
                auto item = groupLayout->takeAt(i);
                if (item != nullptr) {
                    delete item->widget();
                    delete item;
                }
            }
        } else if (mode == Clash::GLOBAL) {  // Display Global group only
            if (groupLayout->count() == 0) {
                ProxyGroupWidget *widget = new ProxyGroupWidget(this);
                widget->updateData(obj["GLOBAL"].toObject());
                groupLayout->addWidget(widget);
            } else {
                auto widget = dynamic_cast<ProxyGroupWidget *>(groupLayout->itemAt(0)->widget());
                widget->updateData(obj["GLOBAL"].toObject());
                // widget->expand();
                while (auto item = groupLayout->takeAt(1)) {
                    delete item->widget();
                    delete item;
                }
            }
        } else {  // Do not display in other mode
            while (auto item = groupLayout->takeAt(0)) {
                delete item->widget();
                delete item;
            }
        }

    } else {
        qDebug() << "Can not parse data: " << rawJson << "Reason: " << error.errorString();
    }
}

void ProxyPage::setMode(Clash::Mode mode) { this->mode = mode; }
