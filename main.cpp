#include "principal.h"
#include <QTimer>
#include <QApplication>
#include <QSplashScreen>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Principal w;
    w.show();
    w.activateWindow();

    return a.exec();
}
