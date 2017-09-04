#include "controlador.h"

Controlador::Controlador(QObject *parent) : QObject(parent)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));

}

void Controlador::executarDownload(QUrl url)
{
    qRegisterMetaType<QList<QSslError>>("__metaTypeObject_QList_QSslError");
    request.setUrl(url);
    QNetworkReply *reply = manager.get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));

}

void Controlador::slotReplyFinished(QNetworkReply *reply)
{
    emit sinalReplyFinished(reply);
}

void Controlador::slotReadyRead()
{
    qDebug() << "Opa!... Ready.. Ready...";
}

void Controlador::slotDownloadProgress(qint64 recebidos, qint64 total)
{
    emit sinalDownloadProgress(recebidos, total);
}

void Controlador::slotSslErrors(QList<QSslError> sslErros)
{
    emit sinalSslErrors(sslErros);
}

void Controlador::slotError(QNetworkReply::NetworkError nErros)
{
    emit slotError(nErros);
}
