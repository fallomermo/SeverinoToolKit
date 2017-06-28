#include "homeinicio.h"
#include "ui_homeinicio.h"

HomeInicio::HomeInicio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeInicio)
{
    ui->setupUi(this);
}

HomeInicio::~HomeInicio()
{
    delete ui;
}
