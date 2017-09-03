#include "blissbeneficios.h"
#include "ui_blissbeneficios.h"

BlissBeneficios::BlissBeneficios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlissBeneficios)
{
    ui->setupUi(this);
}

BlissBeneficios::~BlissBeneficios()
{
    delete ui;
}
