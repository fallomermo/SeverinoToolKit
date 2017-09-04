#ifndef BLISSBENEFICIOS_H
#define BLISSBENEFICIOS_H

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QByteArray>
#include <QDebug>
#include <QTableWidgetItem>
#include <QIODevice>
#include <QMessageBox>

namespace Ui {
class BlissBeneficios;
}

class BlissBeneficios : public QWidget
{
    Q_OBJECT

public:
    explicit BlissBeneficios(QWidget *parent = 0);
    ~BlissBeneficios();

private slots:
    void abrirArquivo();
    void processarArquivoSelecionado();
    void exportarArquivo();
    void processarBlissSeniorTexto();
    void processarBlissEmpresaVTTexto();

private:
    Ui::BlissBeneficios *ui;
};

#endif // BLISSBENEFICIOS_H
