#ifndef GERENCIAMENTOREP_H
#define GERENCIAMENTOREP_H

#include <QWidget>

namespace Ui {
class GerenciamentoREP;
}

class GerenciamentoREP : public QWidget
{
    Q_OBJECT

public:
    explicit GerenciamentoREP(QWidget *parent = 0);
    ~GerenciamentoREP();

private:
    Ui::GerenciamentoREP *ui;
};

#endif // GERENCIAMENTOREP_H
