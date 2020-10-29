#ifndef CLASH_QT_CLASH_H
#define CLASH_QT_CLASH_H

#include "restfulapi.h"
#include <QObject>
#include <QProcess>

namespace Clash{
    enum Mode{
        RULE, GLOBAL, DIRECT
    };

    class Clash : public QProcess{
    Q_OBJECT
    public:
        explicit Clash(QString program = "./clash", QObject *parent = nullptr);
        void start();
    private:
        QString program;
        RestfulApi &restfulApi;
    };
}



#endif //CLASH_QT_CLASH_H
