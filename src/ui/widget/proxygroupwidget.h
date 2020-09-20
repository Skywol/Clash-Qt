#ifndef PROXYGROUPWIDGET_H
#define PROXYGROUPWIDGET_H

#include <QWidget>

namespace Ui {
class ProxyGroupWidget;
}

class ProxyGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProxyGroupWidget(QWidget *parent = nullptr);
    ~ProxyGroupWidget();
    void setExpaned(bool expended);

private:
    Ui::ProxyGroupWidget *ui;
};

#endif // PROXYGROUPWIDGET_H
