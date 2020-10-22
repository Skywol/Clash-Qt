//
// Created by cyril on 2020/10/22.
//

#include "connectionpage.h"
#include "clash/restfulapi.h"
#include "util/instance.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractTableModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHeaderView>

class ConnectionModel : public QAbstractTableModel{
public:
    QModelIndex parent(const QModelIndex &child) const override {
        return QModelIndex();
    }

    int rowCount(const QModelIndex &parent) const override {
        return connections.count();
    }

    int columnCount(const QModelIndex &parent) const override {
        return 5; // start_time, type, source, host, proxy
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal){
            switch (section) {
                case 0: return tr("Start Time");
                case 1: return tr("Network Type");
                case 2: return tr("Source");
                case 3: return tr("Destination");
                case 4: return tr("Proxy");
                default: return QVariant();
            }
        }
        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid()){return QVariant();}
        if(role == Qt::DisplayRole){
            switch (index.column()) {
                case 0: // Start Time
                    return connections.at(index.row())["start"].toString("");
                case 1: return connections.at(index.row())["metadata"]["network"].toString("");
                case 2: return connections.at(index.row())["metadata"]["sourceIP"].toString("") + ":" + connections.at(index.row())["metadata"]["sourcePort"].toString("");
                case 3: return connections.at(index.row())["metadata"]["host"].toString("") +"("
                        + connections.at(index.row())["metadata"]["destinationIP"].toString("") +":"+ connections.at(index.row())["metadata"]["destinationPort"].toString("") +")";
                case 4: return connections.at(index.row())["chains"].toArray().at(0).toString("");
            }
        }
        return QVariant();
    }

    void updateData(QJsonArray data){
        beginResetModel();
        connections = std::move(data);
        endResetModel();
    }

private:
    QJsonArray connections;
    const static QStringList header;
};

ConnectionPage::ConnectionPage(QWidget *parent) :QWidget(parent){
    auto layout = new QVBoxLayout;
    this->setLayout(layout);

    model = new ConnectionModel;
    table = new QTableView;
    table->verticalHeader()->hide();
    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );
    layout->addWidget(table);

    connect(&getInstance<Clash::RestfulApi>(), &Clash::RestfulApi::connectionDataReceived, this, [this](QByteArray json){
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(json, &error);
        if (error.error == QJsonParseError::NoError){
            auto obj = document.object();
            auto connectionArray = obj["connections"];
            if (connectionArray.isArray()){
                model->updateData(connectionArray.toArray());
            }
        } else{
            qDebug()<<"Fail to parse Connection Data [" + error.errorString() + "]: " + json;
        }
    });
}
