#ifndef PROXYGROUPWIDGET_H
#define PROXYGROUPWIDGET_H

#include "ui/util/flowlayout.h"
#include "proxywidget.h"
#include <QWidget>
#include <QJsonObject>

namespace Ui {
class ProxyGroupWidget;
}

class ProxyGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProxyGroupWidget(QWidget *parent = nullptr);
    ~ProxyGroupWidget();
    void setExpanded(bool expended);
    void updateData(const QJsonObject &group);

private:
    Ui::ProxyGroupWidget *ui;
    FlowLayout *flowLayout;
    ProxyWidget *selected;
};

#endif // PROXYGROUPWIDGET_H
