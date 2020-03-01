//
// Created by cyril on 2020/2/23.
//

#include "clashconfig.h"
#include <yaml-cpp/yaml.h>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QCoreApplication>

int ClashConfig::http_port = 7340;
int ClashConfig::socks_port = 7341;
std::string ClashConfig::control_url = "127.0.0.1:9090";

void ClashConfig::loadProfileFromFile(QString filename, YAML::Node &proxy, YAML::Node &proxy_group, YAML::Node &rules){
    if(!QFile().exists(filename)){
        return;
    }
    try {
        YAML::Node node = YAML::LoadFile(filename.toStdString());
        YAML::Node proxyList = node["Proxy"];
        for (auto profile:proxyList) {
            proxy.push_back(profile);
        }

        YAML::Node groupList = node["Proxy Group"];
        for (auto group: groupList) {
            proxy_group.push_back(group);
        }
        YAML::Node rule_list = node["Rule"];
        for (auto rule:rule_list){
            rules.push_back(rule);
        }
    } catch (YAML::Exception e){
        qDebug()
            <<QString("YAML-CPP(line %1, column %2): ").arg(e.mark.line).arg(e.mark.column)
            <<QString::fromStdString(e.msg);
    }
}

void ClashConfig::loadProfileFromString(QString data, YAML::Node &proxy, YAML::Node &proxy_group, YAML::Node &rules){
    try {
        YAML::Node node = YAML::Load(data.toStdString());
        YAML::Node proxyList = node["Proxy"];
        for (auto profile:proxyList) {
            proxy.push_back(profile);
        }

        YAML::Node groupList = node["Proxy Group"];
        for (auto group: groupList) {
            proxy_group.push_back(group);
        }
        YAML::Node rule_list = node["Rule"];
        for (auto rule:rule_list){
            rules.push_back(rule);
        }
    } catch (YAML::Exception e){
        qDebug()
                <<QString("YAML-CPP(line %1, column %2): ").arg(e.mark.line).arg(e.mark.column)
                <<QString::fromStdString(e.msg);
    }
}

bool ClashConfig::combineConfig() {
    QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    //Read Clash Config File
    QFile clash_config_file = QFile(config_path + "/clash-config.yaml");
    // Generate default Clash Config if not exist
    if(not clash_config_file.exists()){
        bool copy_ok = QFile::copy(":/default/clash-config.yaml", config_path+"/clash-config.yaml");
        if(!copy_ok) return false;
    }
    clash_config_file.open(QIODevice::ReadOnly);
    QByteArray clash_config_data = clash_config_file.readAll();
    clash_config_file.close();

    // Read Clash Proxy Profiles
    YAML::Node proxy;
    YAML::Node proxy_group;
    YAML::Node rules;
    loadProfileFromFile(config_path + "/subscribe.yaml", proxy, proxy_group, rules);
    loadProfileFromFile(config_path + "/custom.yaml", proxy, proxy_group, rules);
    YAML::Emitter emitter;
    YAML::Node node;
    node["Proxy"]=proxy;
    node["Proxy Group"]=proxy_group;
    node["Rule"] = rules;
    emitter<<node;

    //Write to clash config file
    QFile config_file(config_path + "/config.yaml");
    config_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&config_file);
    out <<clash_config_data<<"\n"
        <<("external-ui: " + QCoreApplication::applicationDirPath() + "/yacd").toUtf8();
    out<<"\n"<<QString(emitter.c_str());  // If output out.c_str() directly, it will get a wrong codec for non-asiic.

    config_file.close();
    return true;
}

void ClashConfig::genreateCountryDB() {
    QFile::copy(
            ":/default/Country.mmdb",
            QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/Country.mmdb"
            );
}
