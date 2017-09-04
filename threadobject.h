#ifndef THREADOBJECT_H
#define THREADOBJECT_H


#include <QThread>
#include <QDebug>

class ThreadObject : public QThread
{
public:
    ThreadObject();

protected:
    virtual void run();
};

#endif // THREADOBJECT_H
