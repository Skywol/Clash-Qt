#ifndef CLASH_QT_CLASH_H
#define CLASH_QT_CLASH_H

#include <QObject>
#include <QProcess>

class Clash : public QObject{
    Q_OBJECT

signals:
    void errorHappened();
    void readyRead(QByteArray o);
public:
    Clash();
    void start();
    void stop();
    static void generateConfig();
    void restart();
private:
    QProcess *proc;
};


#endif //CLASH_QT_CLASH_H
