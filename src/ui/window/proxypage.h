#ifndef PROXYPAGE_H
#define PROXYPAGE_H

#include <QMap>
#include <QTreeWidgetItem>
#include <QWidget>

namespace Ui {
class ProxyPage;
}

class ProxyPage : public QWidget {
    Q_OBJECT

public:
    const static QStringList display_type;

    explicit ProxyPage(QWidget *parent = nullptr);
    ~ProxyPage();
    void updateProxies(QByteArray rawjson);
    void updateGroup(const QJsonObject &proxies, QString group, QTreeWidgetItem *item);

private:
    Ui::ProxyPage *ui;
    QStringList groups;
    QMap<QString, QTreeWidgetItem *> group_items;
    QMap<QString, QStringList> group_proxies;
};

#endif  // PROXYPAGE_H
