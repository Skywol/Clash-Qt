#ifndef PROXYGROUPWIDGET_H
#define PROXYGROUPWIDGET_H

#include <KCollapsibleGroupBox>
#include <QJsonObject>

#include "proxywidget.h"
#include "ui/util/flowlayout.h"


class ProxyGroupWidget : public KCollapsibleGroupBox {
    Q_OBJECT

public:
    explicit ProxyGroupWidget(QWidget *parent = nullptr);
    void updateData(const QJsonObject &group);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    FlowLayout *flowLayout;
};

#endif  // PROXYGROUPWIDGET_H
