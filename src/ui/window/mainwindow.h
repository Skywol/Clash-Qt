#ifndef CLASH_QT_MAINWINDOW_H
#define CLASH_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

#include "clash/clash.h"

class NetSpeedLabel;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

    static MainWindow &getInstance() {
        static MainWindow instance;
        return instance;
    }

public slots:
    void onTabChange(int index);
    void onClashStarted();

private:
    QTabWidget *tabWidget;
    Clash &clash;

    int proxyTab;
    NetSpeedLabel *netSpeedLabel;
};


#endif  // CLASH_QT_MAINWINDOW_H
