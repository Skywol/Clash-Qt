#ifndef PROXYWIDGET_H
#define PROXYWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

namespace Ui {
class ProxyWidget;
}

class ProxyWidget : public QWidget {
    Q_OBJECT
public:
    explicit ProxyWidget(const QString &groupName = "", const QString &name = "", const QString &type = "", QWidget *parent = nullptr);
    QString getName();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

protected:
private:
    Ui::ProxyWidget *ui;
    QString group;
    QString name;
};

#endif  // PROXYWIDGET_H
