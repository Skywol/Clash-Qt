#ifndef SUBSCRIBEMANAGER_H
#define SUBSCRIBEMANAGER_H

#include <QDialog>
#include <QAbstractTableModel>
#include <QThread>

namespace Ui {
class SubscribeManager;
}

class ProfileModel;
class UpdateThread;

class SubscribeManager : public QDialog
{
    Q_OBJECT
signals:
    void updateFinish(int success, int err);

public:
    explicit SubscribeManager(QWidget *parent = nullptr);
    ~SubscribeManager();
    void updateSubscribe();
    void addSubscribe();

private:
    Ui::SubscribeManager *ui;
    ProfileModel *model;
    UpdateThread *updateThread;
};

class ProfileModel: public QAbstractTableModel
{
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void addSubscribe(QString name, QString url, int index = -1);
    void removeSubscribe(int row);

    QStringList getSubscribeUrl();
private:
    QList<QPair<QString, QString>> profiles;
};

class QNetworkAccessManager;
class UpdateThread:public QThread
{
    Q_OBJECT
signals:
    void updateFinished(int success, int err);
public:
    UpdateThread();
    void setUrlList(QStringList urls);

protected:
    void run() override;

private:
    QStringList urls;
    QNetworkAccessManager *networkAccessManager;
};
#endif // SUBSCRIBEMANAGER_H
