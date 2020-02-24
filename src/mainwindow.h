#ifndef CLASH_QT_MAINWINDOW_H
#define CLASH_QT_MAINWINDOW_H

#include <QMainWindow>

class QWebEngineView;
class SystemTray;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void reload();
private:
    QWebEngineView *view;
};


#endif //CLASH_QT_MAINWINDOW_H
