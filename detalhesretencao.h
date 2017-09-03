#ifndef DETALHESRETENCAO_H
#define DETALHESRETENCAO_H

#include <QDialog>

namespace Ui {
class DetalhesRetencao;
}

class DetalhesRetencao : public QDialog
{
    Q_OBJECT

public:
    explicit DetalhesRetencao(QWidget *parent = 0);
    ~DetalhesRetencao();

private:
    Ui::DetalhesRetencao *ui;
};

#endif // DETALHESRETENCAO_H
