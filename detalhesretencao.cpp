#include "detalhesretencao.h"
#include "ui_detalhesretencao.h"

DetalhesRetencao::DetalhesRetencao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetalhesRetencao)
{
    ui->setupUi(this);
}

DetalhesRetencao::~DetalhesRetencao()
{
    delete ui;
}
