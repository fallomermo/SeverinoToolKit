#ifndef EXPORTARARQUIVO_H
#define EXPORTARARQUIVO_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QByteArray>
#include <QFileDialog>
#include <QTextStream>
#include <QTableWidget>
#include <QProgressDialog>
#include <QTableWidgetItem>

class ExportarArquivo : public QWidget
{
    Q_OBJECT
public:
    explicit ExportarArquivo(QWidget *parent = nullptr);
    ExportarArquivo(QWidget *parent, QTableWidget*);

    void exportar(QString);

signals:
    void mensagemRetorno(QString);

private:
    QTableWidget *table;
};

#endif // EXPORTARARQUIVO_H
