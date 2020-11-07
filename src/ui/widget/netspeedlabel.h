//
// Created by cyril on 2020/10/13.
//

#ifndef CLASH_QT_NETSPEEDLABEL_H
#define CLASH_QT_NETSPEEDLABEL_H

#include <QLabel>
#include <QWidget>

class NetSpeedLabel : public QWidget {
    Q_OBJECT
public:
    NetSpeedLabel(QWidget *parent = nullptr);
    void setSpeed(int up, int down);

private:
    QLabel *up_label, *up_data;
    QLabel *down_label, *down_data;
};


#endif  // CLASH_QT_NETSPEEDLABEL_H
