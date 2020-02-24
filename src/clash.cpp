#include "clash.h"
#include "clashconfig.h"
#include <yaml-cpp/yaml.h>
#include <QProcess>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>
#include <QtWidgets/QTextEdit>

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
}

void Clash::start() {
    if(not QFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/config.yaml").exists()){
        ClashConfig::combineConfig();
    }

    //update clash config information
    YAML::Node clash_config_node = YAML::LoadFile(
            (QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/config.yaml").toStdString()
            );
    ClashConfig::http_port = clash_config_node["port"].as<int>();
    ClashConfig::socks_port = clash_config_node["socks-port"].as<int>();
    ClashConfig::control_url = clash_config_node["external-controller"].as<std::string>();

    if(not QFile(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/Country.mmdb").exists()){
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
        }
        proc->start();
    }
}
