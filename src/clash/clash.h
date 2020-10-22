#ifndef CLASH_QT_CLASH_H
#define CLASH_QT_CLASH_H

#include <QObject>
#include <QProcess>

namespace Clash{
    enum Mode{
        RULE, GLOBAL, DIRECT
    };

    class Clash : public QProcess{
    Q_OBJECT
    public:
        explicit Clash(QObject *parent = nullptr);

        void start();
        void stop();
    signals:
        void started();
        void stoped();
    private:
        QString program;
        QProcess *process;
    };
}



#endif //CLASH_QT_CLASH_H
