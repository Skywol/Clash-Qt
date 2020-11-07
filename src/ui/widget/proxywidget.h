#ifndef PROXYWIDGET_H
#define PROXYWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class ProxyWidget : public QWidget {
    Q_OBJECT
public:
    explicit ProxyWidget(const QString &groupName = "", const QString &name = "", QWidget *parent = nullptr);
    QString getName();
    void setName(const QString &name);
    void setGroup(const QString &group);
    void setChecked(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool checked;
    QHBoxLayout *layout;
    QLabel *proxy, *lag;
};

#endif  // PROXYWIDGET_H
