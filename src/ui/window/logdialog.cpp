#include "logdialog.h"

#include <QScrollBar>
#include <QTime>

#include "clash/clash.h"
#include "ui_logdialog.h"
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

LogDialog::LogDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LogDialog) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    model = new LogTableModel;
    ui->logTabel->setModel(model);
    ui->logTabel->verticalHeader()->hide();
    ui->logTabel->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->clear, &QPushButton::released, model, &LogTableModel::clear);

    connect(getInstance<Clash>().api(), &Clash::RestfulApi::logReceived, this, &LogDialog::addLog);
    getInstance<Clash>().api()->listenLog();
}

LogDialog::~LogDialog() {
    delete ui;
    delete model;
    getInstance<Clash>().api()->stopListenLog();
}
void LogDialog::addLog(QString type, QString content) {
    auto bar = ui->logTabel->verticalScrollBar();
    bool toBottom = bar->value() == bar->maximum();
    model->addLog(type, content);
    if (toBottom) {
        ui->logTabel->scrollToBottom();
    }
}
