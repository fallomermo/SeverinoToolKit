#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QList>
#include <QSsl>
#include <QSslError>
#include <QFile>
#include <QFileDevice>
#include <QFileDialog>

class Controlador : public QObject
{
    Q_OBJECT
public:
    explicit Controlador(QObject *parent = nullptr);

signals:

private slots:

};

#endif // CONTROLADOR_H
