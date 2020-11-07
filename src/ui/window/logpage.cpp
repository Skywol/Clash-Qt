//
// Created by cyril on 2020/10/20.
//

#include "logpage.h"

#include <QAbstractTableModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QTime>
#include <QVBoxLayout>

#include "clash/clash.h"
#include "util/instance.h"

class LogTableModel : public QAbstractTableModel {
    class Log {
    public:
        Log(QString &type, QString &payload) : type(type), payload(payload) { time = QTime::currentTime(); }
        QString getTime() const { return time.toString(); }
        QString getType() const { return type; }
        QString getPayload() const { return payload; }

    private:
        QString type, payload;
        QTime time;
    };

public:
    QModelIndex parent(const QModelIndex &child) const override { return QModelIndex(); }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return tr("Time");
                case 1:
                    return tr("Type");
                case 2:
                    return tr("Payload");
                default:
                    return QVariant();
            }
        }
        return QVariant();
    }

    int rowCount(const QModelIndex &parent) const override { return logs.count(); }

    int columnCount(const QModelIndex &parent) const override {
        return 3;  // time, type, payload;
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid()) {
            return QVariant();
        }

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0:
                    return logs.at(index.row()).getTime();
                case 1:
                    return logs.at(index.row()).getType();
                case 2:
                    return logs.at(index.row()).getPayload();
            }
        }

        return QVariant();
    }

    void addLog(QString &type, QString &payload) {
        beginInsertRows(QModelIndex(), logs.count(), logs.count());
        logs.push_back(Log(type, payload));
        endInsertRows();
    }

    void clear() {
        beginResetModel();
        logs.clear();
        endResetModel();
    }

private:
    QList<Log> logs;
};

LogPage::LogPage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    clearBtn = new QPushButton(tr("Clear"));
    stopBtn = new QPushButton(tr("Stop"));
    headerLayout->addStretch();
    headerLayout->addWidget(clearBtn);
    headerLayout->addWidget(stopBtn);

    layout->addLayout(headerLayout);

    table = new QTableView;
    model = new LogTableModel;

    table->setModel(model);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(table);

    connect(getInstance<Clash>().api(), &Clash::RestfulApi::logReceived, this, [this](QString type, QString payload) {
        auto bar = table->verticalScrollBar();
        bool toBottom = bar->value() == bar->maximum();
        model->addLog(type, payload);
        if (toBottom) {
            table->scrollToBottom();
        }
    });
}
