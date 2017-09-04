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
    QNetworkAccessManager manager;
    QNetworkRequest request;

public:
    explicit Controlador(QObject *parent = nullptr);

signals:
    void sinalReplyFinished(QNetworkReply*);
    void sinalDownloadProgress(qint64,qint64);
    void sinalSslErrors(QList<QSslError>);
    void sinalSlotError(QNetworkReply::NetworkError);
    void sinalLogRegistros(QString);

private slots:
    void executarDownload(QUrl);
    void slotReplyFinished(QNetworkReply*);
    void slotReadyRead();
    void slotDownloadProgress(qint64, qint64);
    void slotSslErrors(QList<QSslError>);
    void slotError(QNetworkReply::NetworkError);

};

#endif // CONTROLADOR_H
