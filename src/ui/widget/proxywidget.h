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
    explicit ProxyWidget(const QString &groupName = "", const QString &name = "",QWidget *parent = nullptr);
    ~ProxyWidget();
    QString getName();
    void setName(const QString &name);
    void setGroup(const QString &group);
    void setChecked(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::ProxyWidget *ui;
    bool checked;
};

#endif // PROXYWIDGET_H
