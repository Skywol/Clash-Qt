//
// Created by cyril on 2020/9/17.
//

#include "clash.h"

Clash::Clash::Clash(QObject *parent){
    process = new QProcess(this);
    process->setProgram("./clash");
}

void Clash::Clash::start() {
    process->start();
}

void Clash::Clash::stop() {
    process->kill();
}
