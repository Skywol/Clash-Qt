#ifndef PROXYGROUPWIDGET_H
#define PROXYGROUPWIDGET_H

#include <QWidget>

namespace Ui {
class ProxyGroupWidget;
}

class ProxyGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit ProxyGroupWidget(QString group = "", QString selected = "", QString type = "", QWidget* parent = nullptr);
    ~ProxyGroupWidget() override;

    void setSelected(const QString& selected);

private:
    Ui::ProxyGroupWidget* ui;
};

#endif  // PROXYGROUPWIDGET_H
