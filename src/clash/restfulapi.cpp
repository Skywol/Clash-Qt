#include "restfulapi.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#define TRAFFIC_URL url + "traffic"
#define LOG_URL url + "logs"
#define PROXY_URL url + "proxies"
#define CONNECTION_URL url + "connections"

Clash::RestfulApi::RestfulApi() :manager(this){
    log = traffic = connection = nullptr;
    url = "http://127.0.0.1:9090/";

    proxyTimer = new QTimer;
    connect(proxyTimer, &QTimer::timeout, this, &RestfulApi::updateProxy);
    connectionTimer = new QTimer;
    connect(connectionTimer, &QTimer::timeout, this, &RestfulApi::updateConnection);
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

void Clash::RestfulApi::updateConnection() {
    QNetworkRequest request;
    request.setRawHeader("Content-Type","application/json");
    request.setUrl(CONNECTION_URL);
    auto reply = manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        emit connectionDataReceived(reply->readAll());
        reply->deleteLater();
    });
}

void Clash::RestfulApi::listenTraffic() {
    if (connection != nullptr && traffic->isRunning()){return;}
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
}

void Clash::RestfulApi::stopListenTraffic() {
    checkReply(traffic);
}

void Clash::RestfulApi::listenLog() {
    if (connection != nullptr && log->isRunning()){
        return;
    }
    QNetworkRequest request;
    request.setRawHeader("Content-Type","application/json");
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

                    if (obj["type"].toString() == "error"){
                        emit errorHappened(obj["payload"].toString());
                    }
                }
            } else{
                qDebug()<<"Failed to parse log data:\n    Data:"
                        <<line<<"\n  Reason:"<<error.errorString();
            }
        }
    });
}

void Clash::RestfulApi::stopListenLog() {
    checkReply(log);
}

void enableTimer(QTimer *timer, bool enable, int interval_ms){
    if (enable){
        timer->setInterval(interval_ms);
        if (!timer->isActive()){
            timer->start();
        }
    } else{
        timer->stop();
    }
}

void Clash::RestfulApi::autoUpdateProxy(bool enable, int interval_ms) {
    enableTimer(proxyTimer, enable, interval_ms);
}

void Clash::RestfulApi::autoUpdateConnection(bool enable, int interval_ms) {
    enableTimer(connectionTimer, enable, interval_ms);
}
