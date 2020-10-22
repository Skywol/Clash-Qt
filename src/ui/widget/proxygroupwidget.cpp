#include "proxygroupwidget.h"
#include "proxywidget.h"
#include "ui/util/flowlayout.h"

#include <QJsonArray>
#include <QSizePolicy>
#include <QDebug>

ProxyGroupWidget::ProxyGroupWidget(QWidget *parent) :
    KCollapsibleGroupBox(parent)
{
    flowLayout = new FlowLayout;
    setLayout(flowLayout);
}

void ProxyGroupWidget::updateData(const QJsonObject &group) {
    int total= flowLayout->count();
    QString groupName = group["name"].toString();
    bool groupChanged = title() != groupName;
    if(groupChanged){
        setTitle(groupName);
        flowLayout->removeAllFrom(0);
        total = 0;
    }
    int index = 0;
    QString now = group["now"].toString();
    for(auto proxy:group["all"].toArray()){
        QString name = proxy.toString();
        ProxyWidget *widget = nullptr;
        if(index >= total){
            widget = new ProxyWidget(groupName, name, this);
            flowLayout->addWidget(widget);
        } else {
            widget = dynamic_cast<ProxyWidget*>(flowLayout->itemAt(index)->widget());
            if(name != widget->property("name")){
                widget = new ProxyWidget(groupName, name, this);
                flowLayout->insertWidget(index, widget);
            } else if(groupChanged){
                widget->setGroup(groupName);
            }
        }
        widget->setChecked(widget->property("name") == now);
        index++;
    }
    flowLayout->removeAllFrom(index);
    update();
}

void ProxyGroupWidget::resizeEvent(QResizeEvent *event) {
    KCollapsibleGroupBox::resizeEvent(event);
    this->flowLayout->update();
}
