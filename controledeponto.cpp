#include "controledeponto.h"
#include "ui_controledeponto.h"

ControleDePonto::ControleDePonto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControleDePonto)
{
    ui->setupUi(this);
}

ControleDePonto::~ControleDePonto()
{
    delete ui;
}
