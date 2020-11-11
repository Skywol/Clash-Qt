#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

namespace Ui {
class LogDialog;
}
class LogTableModel;

class LogDialog : public QDialog {
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = nullptr);
    ~LogDialog() override;

public slots:
    void addLog(QString type, QString content);

private:
    Ui::LogDialog *ui;
    LogTableModel *model;
};

#endif  // LOGDIALOG_H
