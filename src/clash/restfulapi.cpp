#include "restfulapi.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#define TRAFFIC_URL url + "traffic"
#define LOG_URL url + "logs"
#define PROXY_URL url + "proxies"

Clash::RestfulApi::RestfulApi() :manager(this), log(nullptr),traffic(nullptr){
    url = "http://127.0.0.1:9090/";
    start();
}

inline void checkReply(QNetworkReply *reply){
    if(reply != nullptr){
        reply->abort();
        reply->deleteLater();
        reply = nullptr;
    }
}

void checkReply(const QVector<QNetworkReply*> &repliesList){
    for(auto reply:repliesList){
        checkReply(reply);
    }
}

void Clash::RestfulApi::start() {
    checkReply({log, traffic});
    QNetworkRequest request;
    request.setRawHeader("Content-Type","application/json");
    request.setUrl(QUrl(TRAFFIC_URL));

    traffic = manager.get(request);
    connect(traffic, &QNetworkReply::readyRead, this, [this]{
        while (traffic->canReadLine()){
            QJsonParseError error;
            QByteArray line = traffic->readLine();
            auto document = QJsonDocument::fromJson(line, &error);
            if(error.error == QJsonParseError::NoError){
                if(document.isObject()){
                    auto obj = document.object();
                    emit trafficUpdate(obj["up"].toInt(0), obj["down"].toInt(0));
                }
            } else{
                qDebug()<<"Failed to parse traffic data:\n    Data:"
                        <<line<<"\n  Reason:"<<error.errorString();
            }
        }
    });

    request.setUrl(LOG_URL);
    log = manager.get(request);
    connect(log, &QNetworkReply::readyRead, this, [this]{
        while (log->canReadLine()){
            QJsonParseError error;
            QByteArray line = log->readLine();
            auto document = QJsonDocument::fromJson(line, &error);
            if(error.error == QJsonParseError::NoError){
                if(document.isObject()){
                    auto obj = document.object();
                    emit logReceived(obj["type"].toString(), obj["payload"].toString());
                }
            } else{
                qDebug()<<"Failed to parse log data:\n    Data:"
                        <<line<<"\n  Reason:"<<error.errorString();
            }
        }
    });
}

void Clash::RestfulApi::stop() {
    checkReply({log, traffic});
}

void Clash::RestfulApi::updateProxySelector(QString group, QString name) {
    QNetworkRequest request;
    request.setRawHeader("Content-Type","application/json");
    request.setUrl(QUrl(PROXY_URL +"/" + group.toUtf8()));
    QJsonObject data;
    data["name"]=name;
    qDebug()<<request.url()<<QJsonDocument(data).toJson();
    auto reply = manager.put(request, QJsonDocument(data).toJson(QJsonDocument::JsonFormat::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        qDebug()<<reply->readAll();
        updateProxy();
        reply->deleteLater();
    });
}

void Clash::RestfulApi::updateProxy() {
    QNetworkRequest request;
    request.setRawHeader("Content-Type","application/json");
    request.setUrl(QUrl(PROXY_URL));
    auto reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        emit proxyDataReceived(reply->readAll());
        reply->deleteLater();
    });
}
