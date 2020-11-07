#include "proxypage.h"

#include <clash/clash.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "ui/widget/proxygroupwidget.h"
#include "ui/widget/proxywidget.h"
#include "ui_proxypage.h"
#include "util/instance.h"

ProxyPage::ProxyPage(QWidget *parent) : QWidget(parent), ui(new Ui::ProxyPage) {
    ui->setupUi(this);
    connect(getInstance<Clash>().api(), &Clash::RestfulApi::proxyDataReceived, this, &ProxyPage::updateProxies);
}

ProxyPage::~ProxyPage() { delete ui; }
void ProxyPage::updateProxies(QByteArray rawjson) {
    QJsonParseError error{};
    QJsonDocument document = QJsonDocument::fromJson(rawjson, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse proxy data" << error.errorString();
        qDebug() << rawjson;
        return;
    }
    QJsonObject obj = document.object();
    if (!obj.contains("proxies")) {
        qDebug() << "Error Proxy Data. No proxies found";
        return;
    }
    QJsonObject proxies = obj["proxies"].toObject();
    QStringList groupList = proxies.keys();
    if (groups == groupList) {
        // group not change
        for (auto groupName : groupList) {
            auto item = group_items.value(groupName);
            if (item == nullptr) {
                continue;
            }
            updateGroup(proxies, groupName, item);
        }
    } else {
        // clear data if group changed
        ui->treeWidget->clear();
        group_items.clear();
        this->groups = groupList;
        for (const auto &groupName : groups) {
            auto proxyGroup = proxies[groupName].toObject();
            QString type = proxyGroup["type"].toString();
            if (type != "Selector") {
                continue;
            }
            ProxyGroupWidget *groupWidget = new ProxyGroupWidget(groupName, proxyGroup["now"].toString("NULL"), type, this);
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            group_items[groupName] = item;
            if (groupName == "Proxy") {
                ui->treeWidget->insertTopLevelItem(0, item);
            } else {
                ui->treeWidget->insertTopLevelItem(ui->treeWidget->topLevelItemCount(), item);
            }
            ui->treeWidget->setItemWidget(item, 0, groupWidget);
            updateGroup(proxies, groupName, item);
        }
    }
}
void ProxyPage::updateGroup(const QJsonObject &proxies, QString group, QTreeWidgetItem *item) {
    auto array = proxies[group].toObject()["all"].toArray();
    QStringList list;
    for (auto it = array.begin(); it != array.end(); it++) {
        list.append((*it).toString("Error"));
    }
    if (group_proxies.value(group) == list) {
        // proxies in this group not changed.
    } else {
        group_proxies[group] = list;
        // remove previous data;
        for (int i = 0; i < item->childCount(); ++i) {
            auto child = item->child(i);
            delete ui->treeWidget->itemWidget(child, 0);
            item->removeChild(child);
            delete child;
        }
        // add new data
        for (const auto &proxy_name : list) {
            ProxyWidget *widget = new ProxyWidget(group, proxy_name, "null", this);
            QTreeWidgetItem *child = new QTreeWidgetItem(item);
            item->addChild(child);
            ui->treeWidget->setItemWidget(child, 0, widget);
        }
    }
    QString selected = proxies[group].toObject()["now"].toString("NAN");
    static_cast<ProxyGroupWidget *>(ui->treeWidget->itemWidget(item, 0))->setSelected(selected);
    for (int i = 0; i < item->childCount(); ++i) {
        auto child_item = item->child(i);
        auto proxy_widget = static_cast<ProxyWidget *>(ui->treeWidget->itemWidget(child_item, 0));
        child_item->setSelected(proxy_widget->getName() == selected);
    }
}
