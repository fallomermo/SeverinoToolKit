#ifndef TRUNCARARQUIVOSTEXTO_H
#define TRUNCARARQUIVOSTEXTO_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class TruncarArquivosTexto;
}

class TruncarArquivosTexto : public QWidget
{
    Q_OBJECT

public:
    explicit TruncarArquivosTexto(QWidget *parent = 0);
    ~TruncarArquivosTexto();

private slots:
    void abrir();
    void processar();
    void salvar();

private:
    Ui::TruncarArquivosTexto *ui;
};

#endif // TRUNCARARQUIVOSTEXTO_H
