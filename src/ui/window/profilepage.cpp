#include "profilepage.h"

#include <yaml-cpp/yaml.h>

#include <QAbstractTableModel>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QUrl>
#include <QtCore/QDir>

#include "clash/clash.h"
#include "ui_profilepage.h"
#include "util/instance.h"

struct Profile {
    QString name;
    QString url;
    QString file;
    QDateTime updatedTime;
    int interval;
};

template <>
struct YAML::convert<Profile> {
    static Node encode(const Profile &rhs) {
        Node node;
        node["name"] = rhs.name.toStdString();
        node["url"] = rhs.url.toStdString();
        node["file"] = rhs.file.toStdString();
        node["time"] = rhs.updatedTime.toString(Qt::ISODate).toStdString();
        node["interval"] = rhs.interval;
        return node;
    }

    static bool decode(const Node &node, Profile &rhs) {
        if (!node.IsMap()) return false;
        rhs.name = QString::fromStdString(node["name"].as<std::string>("ERROR"));
        rhs.url = QString::fromStdString(node["url"].as<std::string>("ERROR"));
        rhs.file = QString::fromStdString(node["file"].as<std::string>("ERROR"));
        rhs.updatedTime = QDateTime::fromString(QString::fromStdString(node["time"].as<std::string>("2017-07-24T15:46:29")), Qt::ISODate);
        rhs.interval = node["interval"].as<int>(-1);
        return true;
    }
};

class ProfileTableModel : public QAbstractTableModel {
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return tr("Sel");
                case 1:
                    return tr("Name");
                case 2:
                    return tr("Last Update");
                case 3:
                    return tr("Interval");
                case 4:
                    return tr("Url");
                default:
                    return QVariant();
            }
        }
        return QVariant();
    }
    QModelIndex parent(const QModelIndex &child) const override { return QModelIndex(); }
    int rowCount(const QModelIndex &parent) const override { return profileList.count(); }
    int columnCount(const QModelIndex &parent) const override { return 5; }  // selected, name, update-time, interval, url
    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid()) {
            return QVariant();
        }
        int row = index.row();
        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0:
                    return selected == row ? "√" : QVariant();
                case 1:
                    return profileList.at(row).name;
                case 2:
                    return profileList.at(row).updatedTime.toString("yyyy/MM/d hh:mm");
                case 3:
                    return profileList.at(row).interval;
                case 4:
                    return profileList.at(row).url;
                default:
                    return QVariant();
            }
        }
        if (role == Qt::TextAlignmentRole && index.column() == 0) {
            return Qt::AlignCenter;
        }
        return QVariant();
    }
    int getCurrent() { return selected; }
    const Profile &getProfile(int index) { return profileList.at(index); }
    void load() {
        QString filename = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/profile/profiles.yaml";
        if (!QFile::exists(filename)) {
            return;
        }
        YAML::Node profileConfig = YAML::LoadFile(filename.toStdString());
        beginResetModel();
        YAML::Node profileListNode = profileConfig["profiles"];
        if (!profileListNode.IsSequence()) {
            qDebug() << "Profile Config Error";
        } else {
            for (YAML::Node node : profileListNode) {
                profileList.append(node.as<Profile>(Profile()));
            }
        }
        selected = profileConfig["selected"].as<int>(-1);
        endResetModel();
    }

    void save() {
        YAML::Node profiles;
        for (auto profile : profileList) {
            profiles.push_back(profile);
        }
        YAML::Node node;
        node["profiles"] = profiles;
        node["selected"] = selected;

        YAML::Emitter out;
        out << node;
        QDir dir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/profile");
        if (!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/profile/profiles.yaml");
        file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
        QTextStream textStream(&file);
        textStream << out.c_str();
        textStream.flush();
        file.close();
    }

    void addProfile(const Profile &profile) {
        beginInsertRows(QModelIndex(), profileList.count(), profileList.count());
        profileList.append(profile);
        endInsertRows();
    }
    void deleteProfile(int row) {
        if (row < 0 || row >= profileList.count()) {
            return;
        }
        if (row == selected) {
            select(-1);
        }
        beginRemoveRows(QModelIndex(), row, row);
        profileList.removeAt(row);
        endRemoveRows();
    }

    void select(int row) {
        if (selected == row) {
            updateConfig();
            return;
        }
        int prev = selected;
        selected = row;
        if (prev >= 0 && prev < profileList.count()) {
            emit dataChanged(this->index(prev, 0), this->index(prev, 0));
        }
        emit dataChanged(this->index(selected, 0), this->index(selected, 0));
        updateConfig();
    }
    void updateConfig() {
        if (selected < 0 || selected >= profileList.count()) {
            return;
        }
    }

private:
    int selected = -1;
    QList<Profile> profileList;
};


ProfilePage::ProfilePage(QWidget *parent) : QWidget(parent), ui(new Ui::ProfilePage) {
    ui->setupUi(this);

    manger = new QNetworkAccessManager(this);

    model = new ProfileTableModel;
    ui->table->setModel(model);
    ui->table->setColumnWidth(0, 48);
    ui->table->setColumnWidth(1, 100);
    ui->table->setColumnWidth(2, 120);
    ui->table->setColumnWidth(3, 60);
    ui->table->setColumnWidth(4, 400);
    model->load();
    connect(ui->table, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        if (index.isValid()) {
            model->select(index.row());
        }
    });
    connect(ui->downBtn, &QPushButton::clicked, this, &ProfilePage::onDownloadBtnClicked);
    connect(QApplication::instance(), &QApplication::aboutToQuit, this, [this] { model->save(); });
    connect(getInstance<Clash>().api(), &Clash::RestfulApi::connected, this, &ProfilePage::restoreProfile);
}

ProfilePage::~ProfilePage() { delete ui; }
void ProfilePage::onDownloadBtnClicked() { downloadProfile(ui->urlInput->text()); }
void ProfilePage::downloadProfile(QString profileUrl, QString filename) {
    QUrl url(profileUrl);
    if (!url.isValid()) {
        qDebug() << "Url is not valid";
        return;
    }
    if (filename.isEmpty()) {
        filename = QString::number(QDateTime::currentMSecsSinceEpoch()).append(".yaml");
    }

    auto reply = manger->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, filename, reply] {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->url() << reply->errorString();
            return;
        }
        QDir dir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/profile");
        if (!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/profile/" + filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        qint64 res = file.write(reply->readAll());
        if (res > 0) {
            Profile profile;
            profile.name = reply->url().host();
            profile.url = reply->url().toString();
            profile.file = filename;
            profile.updatedTime = QDateTime::currentDateTime();
            profile.interval = 0;
            model->addProfile(profile);
        }
        file.close();
    });
}
void ProfilePage::restoreProfile() { model->updateConfig(); }
