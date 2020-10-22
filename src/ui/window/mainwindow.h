#ifndef CLASH_QT_MAINWINDOW_H
#define CLASH_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

#include "clash/restfulapi.h"

class NetSpeedLabel;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

    static MainWindow& getInstance(){
        static MainWindow instance;
        return instance;
    }

public slots:
    void onTabChange(int index);
private:
    QTabWidget *tabWidget;
    Clash::RestfulApi &restfulApi;

    int proxyTab;
    NetSpeedLabel *netSpeedLabel;
};


#endif //CLASH_QT_MAINWINDOW_H
