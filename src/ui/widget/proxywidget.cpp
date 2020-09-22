#include "proxywidget.h"
#include "ui_proxywidget.h"
#include "clash/restfulapi.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <utility>

ProxyWidget::ProxyWidget(const QString &groupName, const QString &name, QWidget *parent) :
    QWidget(parent), checked(false),
    ui(new Ui::ProxyWidget)
{
    setObjectName("ProxyWidget");
    ui->setupUi(this);
    setName(name);
    setGroup(groupName);
    ui->lag->setText("···");
}

ProxyWidget::~ProxyWidget()
{
    delete ui;
}

void ProxyWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    opt.state = checked ? QStyle::State_On : QStyle::State_Off;
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QString ProxyWidget::getName() {
    return ui->name->text();
}

void ProxyWidget::setName(const QString &name) {
    ui->name->setText(name);
    setProperty("name",name);
}

void ProxyWidget::setChecked(bool checked) {
    this->checked = checked;
    update();
}

void ProxyWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->pos().x()<geometry().width() && event->pos().y() < geometry().height()){
        Clash::RestfulApi::getInstance().updateProxySelector(
                property("group").toString(),
                property("name").toString()
                );
    }

}

void ProxyWidget::setGroup(const QString &group) {
    setProperty("group", group);
}



