#ifndef CLASH_QT_MAINWINDOW_H
#define CLASH_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "ui_mainwindow.h"

#include "clash/restfulapi.h"

class MainWindow : public QMainWindow
        , Ui::MainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void onButtonToggled(QAbstractButton *button, bool checked);
private:
    Ui::MainWindow *ui;
    QButtonGroup *group;
    QAbstractButton *currentButton;

    Clash::RestfulApi &restfulApi;
};


#endif //CLASH_QT_MAINWINDOW_H
