#ifndef RELACAOCOLABORADORES_H
#define RELACAOCOLABORADORES_H

#include <QWidget>

namespace Ui {
class RelacaoColaboradores;
}

class RelacaoColaboradores : public QWidget
{
    Q_OBJECT

public:
    explicit RelacaoColaboradores(QWidget *parent = 0);
    ~RelacaoColaboradores();

private:
    Ui::RelacaoColaboradores *ui;
};

#endif // RELACAOCOLABORADORES_H
