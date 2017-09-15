#include "relacaocolaboradores.h"
#include "ui_relacaocolaboradores.h"

RelacaoColaboradores::RelacaoColaboradores(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelacaoColaboradores)
{
    ui->setupUi(this);
}

RelacaoColaboradores::~RelacaoColaboradores()
{
    delete ui;
}
