#include "proxypage.h"
#include "ui_proxypage.h"
#include "clash/restfulapi.h"
#include <ui/widget/proxygroupwidget.h>
#include <QJsonDocument>
#include <QJsonObject>

ProxyPage::ProxyPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProxyPage)
{
    ui->setupUi(this);
    ui->content->layout()->setAlignment(Qt::AlignmentFlag::AlignTop);

    connect(&Clash::RestfulApi::getInstance(), &Clash::RestfulApi::proxyDataReceived, this, &ProxyPage::updateData);
}

ProxyPage::~ProxyPage()
{
    delete ui;
}

void ProxyPage::updateData(QByteArray rawJson) {
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(rawJson, &error);
    if(error.error == QJsonParseError::NoError){
        QJsonObject obj = document.object()["proxies"].toObject();
        if(obj.isEmpty()){
            qDebug()<<"Can not parse data (empty): " << rawJson<<"Reason: ";
            return;
        }
        auto groupList = obj.keys();
        int proxy_index = groupList.indexOf("Proxy");
        auto layout = ui->content->layout();
        for(int i=0; i<groupList.size();i++){
            int target = 0;
            if(i > proxy_index){
                target = i;
            } else if (i < proxy_index){
                target = i+1;
            }
            if(target >= layout->count()){
                ProxyGroupWidget *widget = new ProxyGroupWidget(this);
                widget->updateData(obj[groupList.at(i)].toObject());
                layout->addWidget(widget);
            } else{
                ((ProxyGroupWidget*)layout->itemAt(target)->widget())
                    ->updateData(obj[groupList.at(i)].toObject());
            }
        }
        for(int i = groupList.size(); i < layout->count(); i++){
            auto item = layout->takeAt(i);
            if(item != nullptr){
                delete item->widget();
                delete item;
            }
        }
    } else{
        qDebug()<<"Can not parse data: " << rawJson<<"Reason: "<< error.errorString();
    }
    update();
}
