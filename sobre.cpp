#include "sobre.h"
#include "ui_sobre.h"

Sobre::Sobre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sobre)
{
    ui->setupUi(this);
}

Sobre::~Sobre()
{
    delete ui;
}
