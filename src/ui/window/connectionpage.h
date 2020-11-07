//
// Created by cyril on 2020/10/22.
//

#ifndef CLASH_QT_CONNECTIONPAGE_H
#define CLASH_QT_CONNECTIONPAGE_H

#include <QTableView>
#include <QWidget>

class ConnectionModel;

class ConnectionPage : public QWidget {
    Q_OBJECT

public:
    explicit ConnectionPage(QWidget *parent = nullptr);

private:
    ConnectionModel *model;
    QTableView *table;
};


#endif  // CLASH_QT_CONNECTIONPAGE_H
