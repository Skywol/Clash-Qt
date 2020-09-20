#include "proxypage.h"
#include "ui_proxypage.h"

#include <ui/widget/proxygroupwidget.h>

ProxyPage::ProxyPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProxyPage)
{
    ui->setupUi(this);
    ui->content->layout()->setAlignment(Qt::AlignmentFlag::AlignTop);
    ui->content->layout()->addWidget(new ProxyGroupWidget);
    ui->content->layout()->addWidget(new ProxyGroupWidget);
}

ProxyPage::~ProxyPage()
{
    delete ui;
}
