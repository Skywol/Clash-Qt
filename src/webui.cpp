//
// Created by cyril on 2020/4/7.
//

#include "webui.h"

Webui::Webui() {
    setWindowTitle("Clash-Qt");
    setUrl(QUrl("http://127.0.0.1:9090/ui"));
    resize(800, 500);
}
