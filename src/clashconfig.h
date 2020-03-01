#ifndef CLASH_QT_CLASHCONFIG_H
#define CLASH_QT_CLASHCONFIG_H

#include <QString>
#include <yaml-cpp/node/node.h>

class ClashConfig {
public:
    static int http_port;
    static int socks_port;
    static std::string control_url;
    static bool combineConfig();
    static void genreateCountryDB();
    static void loadProfileFromFile(QString filename, YAML::Node &proxy, YAML::Node &proxy_group, YAML::Node &rules);
    static void loadProfileFromString(QString data, YAML::Node &proxy, YAML::Node &proxy_group, YAML::Node &rules);
};


#endif //CLASH_QT_CLASHCONFIG_H
