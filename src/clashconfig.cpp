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

bool ClashConfig::combineConfig() {
    QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QFile clash_config_file = QFile(config_path + "/clash-config.yaml");
    if(not clash_config_file.exists()){
        bool copy_ok = QFile::copy(":/default/clash-config.yaml", config_path+"/clash-config.yaml");
        if(not copy_ok) return false;
    }

    QDir sub_folder(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/subscribe");
    if(not sub_folder.exists()){
        sub_folder.mkdir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/subscribe");
    }
    auto sub_profiles = sub_folder.entryInfoList(QDir::Files);
    YAML::Node proxy;
    YAML::Node proxy_group;
    YAML::Node rules;

    for(auto fileInfo:sub_profiles) {
        YAML::Node node = YAML::LoadFile(fileInfo.filePath().toStdString());

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
    }

    if(not clash_config_file.open(QIODevice::ReadOnly)){
        return false;
    }

    QFile config_file(config_path + "/config.yaml");
    config_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&config_file);

    QByteArray clash_config_data = clash_config_file.readAll();
    out <<clash_config_data<<"\n"
        <<("external-ui: " + QCoreApplication::applicationDirPath() + "/yacd").toUtf8();

    YAML::Emitter emitter;
    YAML::Node node;
    node["Proxy"]=proxy;
    node["Proxy Group"]=proxy_group;
    node["Rule"] = rules;
    emitter<<node;
    out<<"\n"<<QString(emitter.c_str());  // If output out.c_str() directly, it will get a wrong codec for non-asiic.

    config_file.close();
}

void ClashConfig::genreateCountryDB() {
    QFile::copy(
            ":/default/Country.mmdb",
            QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/Country.mmdb"
            );
}
