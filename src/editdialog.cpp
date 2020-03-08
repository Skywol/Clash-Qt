#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::setData(QString name, QString url) {
    ui->nameInput->setText(name);
    ui->urlInput->setText(url);
}

QPair<QString, QString> EditDialog::getData() {
    return QPair<QString, QString>(ui->nameInput->text(),ui->urlInput->text());
}
