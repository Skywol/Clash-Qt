#include "proxywidget.h"
#include "clash/restfulapi.h"
#include "util/instance.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QSizePolicy>

ProxyWidget::ProxyWidget(const QString &groupName, const QString &name, QWidget *parent) :
    QWidget(parent), checked(false)
{
    setObjectName("ProxyWidget");
    layout = new QHBoxLayout;
    proxy = new QLabel();
    lag = new QLabel("...");

    layout->addWidget(proxy);
    layout->addStretch();
    layout->addWidget(lag);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(280, 50);

    setLayout(layout);
    layout->setMargin(16);

    setName(name);
    setGroup(groupName);
}

void ProxyWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    opt.state = checked ? QStyle::State_On : QStyle::State_Off;
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QString ProxyWidget::getName() {
    return proxy->text();
}

void ProxyWidget::setName(const QString &name) {
    proxy->setText(name);
    setProperty("name",name);
}

void ProxyWidget::setChecked(bool checked) {
    this->checked = checked;
    update();
}

void ProxyWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->pos().x()<geometry().width() && event->pos().y() < geometry().height()){
        getInstance<Clash::RestfulApi>().updateProxySelector(
                property("group").toString(),
                property("name").toString()
        );
    }

}

void ProxyWidget::setGroup(const QString &group) {
    setProperty("group", group);
}



