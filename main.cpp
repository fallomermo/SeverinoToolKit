#include "login.h"
#include "principal.h"

#include <QFile>
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QApplication>

Login *setup ()
{
    static auto app = new Login(nullptr);

    QFile file(QString(":/images/tema.css"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString tema;
        while (!in.atEnd()) {
            QString line = in.readLine();
            tema.append(line);
        }

        file.close();
        app->setStyleSheet(tema);
    }

    return app;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    setup()->showMaximized();

    return a.exec();
}
