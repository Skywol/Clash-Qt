#ifndef PROXYWIDGET_H
#define PROXYWIDGET_H

#include <QWidget>

namespace Ui {
class ProxyWidget;
}

class ProxyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProxyWidget(QString name = "",QWidget *parent = nullptr);
    ~ProxyWidget();
    QString getName();
    void setName(QString name);
    void set(int lag_ms);
    void setChecked(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::ProxyWidget *ui;
    bool checked;
};

#endif // PROXYWIDGET_H
