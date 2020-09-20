#include "proxywidget.h"
#include "ui_proxywidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

ProxyWidget::ProxyWidget(QString name, QWidget *parent) :
    QWidget(parent), checked(false),
    ui(new Ui::ProxyWidget)
{
    setObjectName("ProxyWidget");
    ui->setupUi(this);
    ui->name->setText("Test Proxy");
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

void ProxyWidget::setName(QString name) {
    ui->name->setText(name);
}

void ProxyWidget::set(int lag_ms) {

}

void ProxyWidget::setChecked(bool checked) {
    this->checked = checked;
    update();
}

void ProxyWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(!this->geometry().contains(event->pos())) return;
    setChecked(!checked);
}



