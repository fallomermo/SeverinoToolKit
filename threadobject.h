#ifndef THREADOBJECT_H
#define THREADOBJECT_H


#include <QThread>

class ThreadObject : public QThread
{
public:
    ThreadObject();

private slots:
    void finish();

};

#endif // THREADOBJECT_H
