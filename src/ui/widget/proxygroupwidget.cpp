#include "proxygroupwidget.h"

#include <QDebug>

#include "ui_proxygroupwidget.h"

ProxyGroupWidget::ProxyGroupWidget(QString group, QString selected, QString type, QWidget* parent)
    : QWidget(parent), ui(new Ui::ProxyGroupWidget) {
    ui->setupUi(this);
    ui->group->setText(group);
    ui->selected->setText(selected);
    ui->type->setText(QString("<font color='%1'>%2</font>").arg("grey").arg(type));
}

ProxyGroupWidget::~ProxyGroupWidget() {
    QWidget::~QWidget();
    delete ui;
}
void ProxyGroupWidget::setSelected(const QString& selected) { ui->selected->setText(selected); }
