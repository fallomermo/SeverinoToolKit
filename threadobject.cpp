#include "threadobject.h"

ThreadObject::ThreadObject()
{
    qDebug() << "Instanciando ThreadObject com sucesso!";
}

void ThreadObject::run()
{
    this->exec();
}
