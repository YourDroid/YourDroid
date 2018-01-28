#ifndef CMD_H
#define CMD_H

#include <QString>
#include <QPair>
#include <QThread>

#define CMD_ASSERT(expr) ABORT(expr.second);

class cmd {
    static QThread *mainThread;
public:
    static void init() { mainThread = QThread::currentThread(); }
    static QPair<int, QString> exec(QString, bool = false);
};

#endif // CMD_H
