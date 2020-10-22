//
// Created by cyril on 2020/10/13.
//

#include "netspeedlabel.h"
#include <QHBoxLayout>

NetSpeedLabel::NetSpeedLabel(QWidget *parent):
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);

    up_label = new QLabel(tr("Upload: "));
    down_label = new QLabel(tr("Download: "));
    up_data = new QLabel("--- B/s");
    down_data = new QLabel("--- B/s");

    layout->addWidget(up_label);
    layout->addWidget(up_data);
    layout->addWidget(down_label);
    layout->addWidget(down_data);
}

QString netSpeedStr(int speed){
    const static QStringList uint = {"B/s", "KB/s", "MB/s", "GB/s"};
    float speed_f = speed;
    int count = 0;
    while(speed_f > 1000 && count < uint.size()-1){
        speed_f /= 1024;
        count++;
    }
    return QString::number(speed_f, 'f', 2) + uint.at(count);
}

void NetSpeedLabel::setSpeed(int up, int down) {
    up_data->setText(netSpeedStr(up));
    down_data->setText(netSpeedStr(down));
}
