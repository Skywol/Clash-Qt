#ifndef CLASH_QT_CLASH_H
#define CLASH_QT_CLASH_H

#include <QObject>
#include <QProcess>

class Clash : public QObject{
    Q_OBJECT

signals:
    void errorHappened();
    void readyRead(QByteArray o);
    void clashStarted();
public:
    Clash();
    void start();
    void stop();
    void restart();
private:
    QProcess *proc;
};


#endif //CLASH_QT_CLASH_H
