//
// Created by cyril on 2020/9/17.
//

#include "clash.h"

Clash::Process::Process(QObject *parent) : QObject(parent){
    process.setParent(this);
}
