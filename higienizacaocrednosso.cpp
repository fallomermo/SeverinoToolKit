#include "higienizacaocrednosso.h"
#include "ui_higienizacaocrednosso.h"

HigienizacaoCrednosso::HigienizacaoCrednosso(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HigienizacaoCrednosso)
{
    ui->setupUi(this);
}

HigienizacaoCrednosso::~HigienizacaoCrednosso()
{
    delete ui;
}
