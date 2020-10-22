#ifndef PROXYGROUPWIDGET_H
#define PROXYGROUPWIDGET_H

#include "ui/util/flowlayout.h"
#include "proxywidget.h"
#include <KCollapsibleGroupBox>
#include <QJsonObject>


class ProxyGroupWidget : public KCollapsibleGroupBox
{
    Q_OBJECT

public:
    explicit ProxyGroupWidget(QWidget *parent = nullptr);
    void updateData(const QJsonObject &group);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    FlowLayout *flowLayout;
};

#endif // PROXYGROUPWIDGET_H
