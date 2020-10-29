//
// Created by cyril on 2020/9/17.
//

#include "clash.h"
#include "util/instance.h"
#include <QStandardPaths>

Clash::Clash::Clash(QString program, QObject *parent)
:QProcess(parent), program(program), restfulApi(getInstance<RestfulApi>()){

}

void Clash::Clash::start() {
    setProgram(program);
    QProcess::start();
};