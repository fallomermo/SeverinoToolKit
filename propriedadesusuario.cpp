#include "propriedadesusuario.h"
#include "ui_propriedadesusuario.h"

PropriedadesUsuario::PropriedadesUsuario(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropriedadesUsuario)
{
    ui->setupUi(this);
}

PropriedadesUsuario::~PropriedadesUsuario()
{
    delete ui;
}
