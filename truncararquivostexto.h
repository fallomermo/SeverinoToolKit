#ifndef TRUNCARARQUIVOSTEXTO_H
#define TRUNCARARQUIVOSTEXTO_H

#include <QWidget>

namespace Ui {
class TruncarArquivosTexto;
}

class TruncarArquivosTexto : public QWidget
{
    Q_OBJECT

public:
    explicit TruncarArquivosTexto(QWidget *parent = 0);
    ~TruncarArquivosTexto();

private:
    Ui::TruncarArquivosTexto *ui;
};

#endif // TRUNCARARQUIVOSTEXTO_H
