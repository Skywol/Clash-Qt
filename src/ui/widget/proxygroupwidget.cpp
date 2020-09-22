#include "proxygroupwidget.h"
#include "ui_proxygroupwidget.h"
#include "proxywidget.h"
#include "ui/util/flowlayout.h"

#include <QJsonArray>

ProxyGroupWidget::ProxyGroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProxyGroupWidget), selected(nullptr)
{
    ui->setupUi(this);
    connect(ui->groupButton, &QPushButton::clicked, this, &ProxyGroupWidget::setExpanded);
    setExpanded(true);

    flowLayout = new FlowLayout(ui->content);
}

ProxyGroupWidget::~ProxyGroupWidget()
{
    delete ui;
}

void ProxyGroupWidget::setExpanded(bool expended) {
    ui->groupButton->setIcon(QIcon(expended?":/icon/arrow-down.svg":":/icon/arrow-up.svg"));
    ui->groupButton->setChecked(expended);
    ui->content->setMaximumHeight(expended?INT16_MAX:0);
    update();
}

void ProxyGroupWidget::updateData(const QJsonObject &group) {
    int total= flowLayout->count();
    QString groupName = group["name"].toString();
    bool groupChanged = ui->groupName->text() != groupName;
    if(groupChanged){
        ui->groupName->setText(groupName);
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
    if(index < total){
        flowLayout->removeWidgets(index++);
    }
    update();
}
