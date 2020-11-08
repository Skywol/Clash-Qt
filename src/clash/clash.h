#ifndef CLASH_QT_CLASH_H
#define CLASH_QT_CLASH_H

#include <QObject>
#include <QtCore/QDateTime>
#include <QtCore/QMap>

#include "yaml-cpp/yaml.h"

namespace YAML {
class Node;
}

class QProcess;
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;

class Clash : public QObject {
public:
    struct Profile {
        QString name;
        QString url;
        QString file;
        QDateTime updatedTime;
        int interval;
    };
    class RestfulApi;

public:
    explicit Clash(QString program = "./clash", QString clash_dir = "", QObject *parent = nullptr);
    void start();
    void stop();
    bool checkFiles();
    RestfulApi *api();
    void switchProfile(QString name);

private:
    QProcess *process;
    RestfulApi *restfulApi;
    QString clash_program, clash_dir;
};

class Clash::RestfulApi : public QObject {
    Q_OBJECT
public:
    RestfulApi(QObject *parent = nullptr);

    void testConnection();

    void listenTraffic();
    void stopListenTraffic();

    void listenLog();
    void stopListenLog();

    void updateConfig();
    void updateConnection();
    void updateProxy();
    void updateProxySelector(QString group, QString name, bool update = true);

    void autoUpdateProxy(bool enable = true, int interval_ms = 1000);
    void autoUpdateConnection(bool enable = true, int interval_ms = 1000);
    void autoUpdateConfig(bool enable = true, int interval_ms = 1000);

    void updateProfile(QString filename);
    void patchConfig(QString key, QVariant value);

signals:
    void connected();
    void configUpdate(QByteArray rawJson);
    void trafficUpdate(int up, int down);
    void logReceived(QString type, QString payload);
    void errorHappened(QString content);
    void proxyDataReceived(QByteArray rawJson);
    void connectionDataReceived(QByteArray rawJson);
    void successChangeProfile();
    void failedChangeProfile(QString reason);

private:
    QString url;
    QNetworkAccessManager *manager;
    QNetworkReply *traffic, *log, *connection;
    QTimer *proxyTimer, *connectionTimer, *configTimer;
};


#endif  // CLASH_QT_CLASH_H
