#ifndef CLASH_QT_RESTFULAPI_H
#define CLASH_QT_RESTFULAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Clash{
    class RestfulApi : public QObject{
        Q_OBJECT
    public:
        static RestfulApi& getInstance(){
            static RestfulApi instance;
            return instance;
        }

        void start();
        void stop();
        void updateProxy();
        void updateProxySelector(QString group, QString name);
    signals:
        void trafficUpdate(int up, int down);
        void logReceived(QString type, QString payload);
        void proxyDataReceived(QByteArray rawJson);
    private:
        RestfulApi();
        QString url;
        QNetworkAccessManager manager;
        QNetworkReply *traffic, *log;

    };

}


#endif //CLASH_QT_RESTFULAPI_H
