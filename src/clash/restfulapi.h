#ifndef CLASH_QT_RESTFULAPI_H
#define CLASH_QT_RESTFULAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

namespace Clash{
    class RestfulApi : public QObject{
        Q_OBJECT
    public:
        RestfulApi();

        void listenConfig();
        void stopListenConfig();

        void listenTraffic();
        void stopListenTraffic();

        void listenLog();
        void stopListenLog();

        void updateConnection();
        void updateProxy();
        void updateProxySelector(QString group, QString name);

        void autoUpdateProxy(bool enable = true, int interval_ms = 1000);
        void autoUpdateConnection(bool enable = true, int interval_ms = 1000);

    signals:
        void trafficUpdate(int up, int down);
        void logReceived(QString type, QString payload);
        void errorHappened(QString content);
        void proxyDataReceived(QByteArray rawJson);
        void connectionDataReceived(QByteArray rawJson);
    private:
        QString url;
        QNetworkAccessManager manager;
        QNetworkReply *traffic, *log, *connection;
        QTimer *proxyTimer, *connectionTimer;
    };

}


#endif //CLASH_QT_RESTFULAPI_H
