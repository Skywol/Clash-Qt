#ifndef CLASH_QT_CLASHCONFIG_H
#define CLASH_QT_CLASHCONFIG_H

#include <QString>

class ClashConfig {
public:
    static int http_port;
    static int socks_port;
    static std::string control_url;
    static bool combineConfig();
    static void genreateCountryDB();
};


#endif //CLASH_QT_CLASHCONFIG_H
