#include "profiledialog.h"

#include <QAbstractTableModel>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtCore/QDir>
#include <QtCore/QUrl>

#include "ui_profiledialog.h"

class ProfileTableModel : public QAbstractTableModel {
public:
    explicit ProfileTableModel(Clash::ProfileList &profile_list, QObject *parent)
        : QAbstractTableModel(parent), profile_list(profile_list) {}
    QModelIndex parent(const QModelIndex &child) const override { return QModelIndex(); }
    int rowCount(const QModelIndex &parent) const override { return profile_list.getList().size(); }
    int columnCount(const QModelIndex &parent) const override { return 4; }  // name, update, interval, url
    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid()) {
            return QVariant();
        }
        if (role == Qt::DisplayRole) {
            auto profile = profile_list.getList().at(index.row());
            switch (index.column()) {
                case 0:
                    return profile.name;
                case 1:
                    return profile.updatedTime.toString("yyyy-MM-dd hh:mm");
                case 2:
                    return profile.interval;
                case 3:
                    return profile.url;
                default:
                    return QVariant();
            }
        }
        return QVariant();
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return tr("Name");
                case 1:
                    return tr("Last Update");
                case 2:
                    return tr("Interval");
                case 3:
                    return tr("Url");
                default:
                    return QVariant();
            }
        }
        return QVariant();
    }

    void addProfile(const Clash::Profile &profile) {
        beginInsertRows(QModelIndex(), profile_list.getList().size(), profile_list.getList().size());
        profile_list.getList().append(profile);
        if (profile_list.getIndex() < 0) {
            profile_list.setIndex(0);
        }
        endInsertRows();
    }
    void removeProfile(int index) {
        beginRemoveRows(QModelIndex(), index, index);
        profile_list.remove(index);
        endRemoveRows();
    }

private:
    Clash::ProfileList &profile_list;
};

ProfileDialog::ProfileDialog(Clash::ProfileList &profiles, QWidget *parent)
    : QDialog(parent), ui(new Ui::ProfileDialog), profiles(profiles) {
    ui->setupUi(this);

    model = new ProfileTableModel(profiles, this);
    ui->profileTable->setModel(model);
    ui->profileTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ProfileDialog::downloadFinished);
    connect(ui->download, &QPushButton::released, this, &ProfileDialog::downloadProfile);
}
void ProfileDialog::downloadProfile() {
    QUrl url(ui->url->text());
    if (!url.isValid()) {
        QMessageBox::critical(this, tr("Invalid Url"), url.errorString());
    }
    manager->get(QNetworkRequest(url));
}

void ProfileDialog::downloadFinished(QNetworkReply *reply) {
    qDebug() << "Download finish";
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, tr("Download Profile Failed"), reply->errorString());
        return;
    }

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/profile");
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }

    QString filename = QString::number(QDateTime::currentMSecsSinceEpoch()) + ".yaml";
    QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/profile/" + filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    qint64 res = file.write(reply->readAll());
    if (res > 0) {
        Clash::Profile profile;
        profile.name = reply->url().host();
        profile.url = reply->url().toString();
        profile.file = filename;
        profile.updatedTime = QDateTime::currentDateTime();
        profile.interval = 0;
        model->addProfile(profile);
    }
    file.close();
}
ProfileDialog::~ProfileDialog() {
    delete ui;
    delete manager;
    delete model;
}
