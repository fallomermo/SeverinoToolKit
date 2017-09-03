#ifndef HIGIENIZACAOCREDNOSSO_H
#define HIGIENIZACAOCREDNOSSO_H

#include <QWidget>

namespace Ui {
class HigienizacaoCrednosso;
}

class HigienizacaoCrednosso : public QWidget
{
    Q_OBJECT

public:
    explicit HigienizacaoCrednosso(QWidget *parent = 0);
    ~HigienizacaoCrednosso();

private:
    Ui::HigienizacaoCrednosso *ui;
};

#endif // HIGIENIZACAOCREDNOSSO_H
