//
// Created by cyril on 2020/2/22.
//

#include "mainwindow.h"
#include "systemtray.h"
#include <QDir>
#include <QWebEngineView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    view = new QWebEngineView(this);
    setWindowTitle("Clash-Qt");
    setCentralWidget(view);
    resize(800, 500);
    view->setUrl(QUrl("http://127.0.0.1:9090/ui"));
    view->show();
}

void MainWindow::reload() {
    view->reload();
}
