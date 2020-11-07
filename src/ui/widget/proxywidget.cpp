#include "proxywidget.h"

#include <QDebug>
#include <QMouseEvent>

#include "clash/clash.h"
#include "ui_proxywidget.h"
#include "util/instance.h"

ProxyWidget::ProxyWidget(const QString& groupName, const QString& name, const QString& type, QWidget* parent)
    : QWidget(parent), ui(new Ui::ProxyWidget) {
    ui->setupUi(this);
    ui->name->setText(name);
    ui->type->setText(QString("<font color='%1'>%2</font>").arg("grey").arg(type));

    this->group = groupName;
    this->name = name;
}
QString ProxyWidget::getName() { return name; }
void ProxyWidget::mouseReleaseEvent(QMouseEvent* event) {
    QWidget::mouseReleaseEvent(event);
    QPoint point = event->pos();
    if (rect().contains(point)) {
        static Clash::RestfulApi api = getInstance<Clash>().api();
        api.updateProxySelector(group, name);
    }
}
