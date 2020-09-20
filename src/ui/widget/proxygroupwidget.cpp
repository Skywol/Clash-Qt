#include "proxygroupwidget.h"
#include "ui_proxygroupwidget.h"
#include "proxywidget.h"
#include "ui/util/flowlayout.h"

ProxyGroupWidget::ProxyGroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProxyGroupWidget)
{
    ui->setupUi(this);
    connect(ui->groupButton, &QPushButton::clicked, this, &ProxyGroupWidget::setExpaned);
    setExpaned(false);
    FlowLayout *flowLayout = new FlowLayout;
    ui->content->setLayout(flowLayout);
    for(int i=0;i<20;i++){
        flowLayout->addWidget(new ProxyWidget("Proxy"+QString::number(i)));
    }
}

ProxyGroupWidget::~ProxyGroupWidget()
{
    delete ui;
}

void ProxyGroupWidget::setExpaned(bool expended) {
    ui->groupButton->setIcon(QIcon(expended?":/icon/arrow-down.svg":":/icon/arrow-up.svg"));
    ui->groupButton->setChecked(expended);
    ui->content->setMaximumHeight(expended?INT16_MAX:0);
}
