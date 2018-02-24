#include "gerenciamentorep.h"
#include "ui_gerenciamentorep.h"

GerenciamentoREP::GerenciamentoREP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GerenciamentoREP)
{
    ui->setupUi(this);
}

GerenciamentoREP::~GerenciamentoREP()
{
    delete ui;
}
