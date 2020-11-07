#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
namespace Ui {
class ProfilePage;
}
class QNetworkAccessManager;
class ProfileTableModel;

class ProfilePage : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();
    void downloadProfile(QString profileUrl, QString filename = "");

public slots:
    void onDownloadBtnClicked();
    void restoreProfile();

private:
    Ui::ProfilePage *ui;
    QNetworkAccessManager *manger;
    ProfileTableModel *model;
};

#endif  // PROFILEPAGE_H
