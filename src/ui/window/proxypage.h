#ifndef PROXYPAGE_H
#define PROXYPAGE_H

#include <QHBoxLayout>
#include <QScrollArea>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QWidget>

#include "clash/clash.h"
#include "yaml-cpp/yaml.h"

class ProxyPage : public QWidget {
    Q_OBJECT

public:
    explicit ProxyPage(QWidget *parent = nullptr);

    void updateData(QByteArray rawJson);
    void setMode(Clash::Mode mode);

private:
    YAML::Node *node;
    Clash::Mode mode;
    QHBoxLayout *headerArea;
    QVBoxLayout *groupLayout;
    QScrollArea *groupArea;
};

#endif  // PROXYPAGE_H
