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
    static void load();
    void save();
    void start();
    void stop();
    void restart();
private:
    QProcess *proc;
    qint64 pid;
};


#endif //CLASH_QT_CLASH_H
