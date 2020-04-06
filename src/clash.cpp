#include "clash.h"
#include "clashconfig.h"
#include <yaml-cpp/yaml.h>
#include <QProcess>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>
#include <QtWidgets/QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

Clash::Clash() {
    proc = new QProcess(this);

    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exit_code, QProcess::ExitStatus exitStatus){
                if(exit_code != 0){
                    emit errorHappened();
                }
            }
    );

    proc->setProgram(QCoreApplication::applicationDirPath() + "/clash");
    proc->setArguments(QStringList()
            <<"-d"
            <<QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
    );
    proc->setProcessChannelMode(QProcess::MergedChannels);

    connect(proc, &QProcess::readyRead, this, [this]{
        emit readyRead(proc->readAll());
    });
    connect(proc, &QProcess::started, this, [this]{qDebug()<<"proc started"; emit clashStarted();});
}

void Clash::start() {
    if(!QFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/config.yaml").exists()){
        ClashConfig::combineConfig();
    }

    //update clash config information
    YAML::Node clash_config_node = YAML::LoadFile(
            (QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/config.yaml").toStdString()
            );
    ClashConfig::http_port = clash_config_node["port"].as<int>();
    ClashConfig::socks_port = clash_config_node["socks-port"].as<int>();
    clash_config_node["external-controller"].as<std::string>();

    if(!QFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/Country.mmdb").exists()){
        ClashConfig::genreateCountryDB();
    }
    if(proc != nullptr){
        proc->start();
    }
}

void Clash::restart() {
    ClashConfig::combineConfig();
    if(proc != nullptr){
        if(proc->state() != QProcess::NotRunning ){
            proc->terminate();
            proc->waitForFinished(1000);
        }
        proc->start();
    }
}

void Clash::stop() {
    save();
    proc->terminate();
}

void Clash::save() {
    if(proc->state() == QProcess::NotRunning)
        return;
    qDebug()<<"Save Status";
    QNetworkAccessManager manager;
    QNetworkRequest req(QUrl("http://127.0.0.1:9090/proxies"));
    auto re = manager.get(req);

    QEventLoop loop;
    connect(re, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    auto json = QJsonDocument::fromJson(re->readAll()).object();
    if(json.contains("proxies")){
        YAML::Node proxies_list;
        auto proxies = json["proxies"].toObject();
        for(auto proxy_json:proxies){
            auto obj = proxy_json.toObject();
            if(obj["type"].toString() != "Selector") continue;
            YAML::Node proxy;
            proxy["name"] = obj["name"].toString().toStdString();
            proxy["now"] = obj["now"].toString().toStdString();
            proxies_list.push_back(proxy);
        }
        YAML::Emitter emitter;
        emitter<<proxies_list;
        QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QFile save_file(config_path + "/status.yaml");
        save_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream out(&save_file);
        out<<QString(emitter.c_str());
        save_file.close();
    } else{
        qDebug()<<"Get Status Error!";
    }
}

void Clash::load() {
    qDebug()<<"Load Status";
    QNetworkAccessManager manager;
    QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    YAML::Node proxies = YAML::LoadFile(QString(config_path + "/status.yaml").toStdString());
    for(auto const &proxy:proxies){
        QString name = QString::fromStdString(proxy["name"].as<std::string>());
        QString now = QString::fromStdString(proxy["now"].as<std::string>());
        QNetworkRequest request(QUrl("http://127.0.0.1:9090/proxies/"+name));
        QByteArray data = QString(R"({"name":"%1"})").arg(now).toUtf8();
        auto re = manager.put(request, data);
        QEventLoop loop;
        QEventLoop::connect(re, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        re->deleteLater();
    }
}
