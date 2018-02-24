#include "threadobject.h"

ThreadObject::ThreadObject() {}

void ThreadObject::finish()
{
    if(this->isRunning()) {
        this->deleteLater();
        this->quit();
    }
}

