#ifndef EXPORTARARQUIVO_H
#define EXPORTARARQUIVO_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QByteArray>
#include <QFileDialog>
#include <QTextStream>
#include <QTableWidget>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTableWidgetItem>
#include "caixamensagemprogresso.h"

class ExportarArquivo : public QWidget
{
    Q_OBJECT
public:
    explicit ExportarArquivo(QWidget *parent = nullptr);
    ExportarArquivo(QWidget *parent, QTableWidget*);

    void exportar(QString, int);

signals:
    void progressValue(int);
    void minimumProgressValue(int);
    void maximumProgressValue(int);
    void fecharCaixaDeMensagem();
    void mensagemRetorno(QString);

private:
    QTableWidget *table;
    CaixaMensagemProgresso *caixaMensagem;
};

#endif // EXPORTARARQUIVO_H
