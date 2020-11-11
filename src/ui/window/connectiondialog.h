#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectionDialog;
}

class ConnectionModel;

class ConnectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog() override;

public slots:
    void updateConnection(QByteArray json);

private:
    Ui::ConnectionDialog *ui;
    ConnectionModel *model;
    QTimer *timer;
};

#endif  // CONNECTIONDIALOG_H
