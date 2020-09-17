#ifndef CLASH_QT_CLASH_H
#define CLASH_QT_CLASH_H

#include <QObject>
#include <QProcess>

namespace Clash{
    class Process : public QObject{
    Q_OBJECT
    public:
        explicit Process(QObject *parent = nullptr);

        void start();
        void stop();
        void restart();

    private:
        QString program;
        QProcess process;
    };
}



#endif //CLASH_QT_CLASH_H
