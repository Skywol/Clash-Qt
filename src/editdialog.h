#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr);
    void setData(QString name, QString url);
    QPair<QString,QString> getData();
    ~EditDialog();

private:
    Ui::EditDialog *ui;
};

#endif // EDITDIALOG_H
