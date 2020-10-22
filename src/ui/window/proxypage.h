#ifndef PROXYPAGE_H
#define PROXYPAGE_H

#include "clash/clash.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

class ProxyPage : public QWidget
{
    Q_OBJECT

public:
    explicit ProxyPage(QWidget *parent = nullptr);

    void updateData(QByteArray rawJson);
    void setMode(Clash::Mode mode);
private:

    Clash::Mode mode;
    QHBoxLayout *headerArea;
    QVBoxLayout *groupLayout;
    QScrollArea *groupArea;
};

#endif // PROXYPAGE_H
