#include "subscribemanager.h"
#include "ui_subscribemanager.h"
#include "clashconfig.h"
#include <QUrl>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <yaml-cpp/yaml.h>
#include <QStandardPaths>
#include <QtCore/QFile>

SubscribeManager::SubscribeManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubscribeManager)
{
    ui->setupUi(this);
    model = new ProfileModel;
    ui->subscribeTable->setModel(model);
    ui->subscribeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->subscribeTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->subscribeTable->setColumnWidth(0,100);
    ui->subscribeTable->horizontalHeader()->setStretchLastSection(true);

    updateThread = new UpdateThread();
    connect(updateThread, &UpdateThread::updateFinished, this,
            [this](int success, int err){emit updateFinish(success,err);});
    connect(updateThread, &UpdateThread::started, this, [this]{ui->updateBtn->setEnabled(false);});
    connect(updateThread, &UpdateThread::finished, this, [this]{ui->updateBtn->setEnabled(true);});

    connect(ui->updateBtn, &QPushButton::clicked, this, &SubscribeManager::updateSubscribe);
    connect(ui->addBtn, &QPushButton::clicked, this, &SubscribeManager::addSubscribe);
}

SubscribeManager::~SubscribeManager()
{
    delete ui;
}

void SubscribeManager::updateSubscribe() {
    updateThread->setUrlList(model->getSubscribeUrl());
    updateThread->start();
}

void SubscribeManager::addSubscribe() {
    QString url_str = ui->inputUrl->text();
    QUrl url(url_str);
    if(url.host().isEmpty() || !url.scheme().contains("http", Qt::CaseInsensitive)){
        QMessageBox::warning(this, tr("Url Error"), tr("Input url is not valid!"));
        return;
    }
    model->addSubscribe(url.host(), url_str);
}

int ProfileModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return profiles.size();
}

int ProfileModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 2;
}

QVariant ProfileModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole){
        switch(index.column()){
            case 0: return profiles.at(index.row()).first;
            case 1: return profiles.at(index.row()).second;
        }
    }
    return QVariant();
}

QVariant ProfileModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
        switch (section){
            case 0: return QString(tr("Name"));
            case 1: return QString(tr("Subscribe Url"));
        }
    }
    return QVariant();
}

void ProfileModel::addSubscribe(QString name, QString url, int index) {
    int row = index<0?profiles.size():index;
    beginInsertRows(QModelIndex(), row,row);
    profiles.append(QPair(name, url));
    endInsertRows();
}

void ProfileModel::removeSubscribe(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    profiles.removeAt(row);
    endRemoveRows();
}

QStringList ProfileModel::getSubscribeUrl() {
    QStringList urls;
    for(auto profile:profiles){
        urls.append(profile.second);
    }
    return urls;
}

UpdateThread::UpdateThread() {
    networkAccessManager = new QNetworkAccessManager;
    networkAccessManager->moveToThread(this);
}

void UpdateThread::run() {
    QList<QPair<QString,QString>> errorList;
    YAML::Node proxy;
    YAML::Node proxy_group;
    YAML::Node rules;
    for(const auto& url:urls){
        QNetworkReply *reply = networkAccessManager->get(QNetworkRequest(QUrl(url)));
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec(QEventLoop::ExcludeUserInputEvents);
        if(reply->error()){
            errorList.append(QPair(url, reply->errorString()));
        } else{
            ClashConfig::loadProfileFromString(reply->readAll(), proxy, proxy_group, rules);
        }
        reply->deleteLater();
    }
    QFile subscribeFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/subscribe.yaml");
    subscribeFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&subscribeFile);
    YAML::Node node;
    node["Proxy"]=proxy;
    node["Proxy Group"]=proxy_group;
    node["Rule"] = rules;
    YAML::Emitter emitter;
    emitter<<node;
    out<<QString(emitter.c_str());
    emit updateFinished(urls.size()-errorList.size(), errorList.size());
}

void UpdateThread::setUrlList(QStringList urls) {
    this->urls = urls;
}