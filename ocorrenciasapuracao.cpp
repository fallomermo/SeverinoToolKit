#include "ocorrenciasapuracao.h"
#include "ui_ocorrenciasapuracao.h"

OcorrenciasApuracao::OcorrenciasApuracao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OcorrenciasApuracao)
{
    ui->setupUi(this);
}

OcorrenciasApuracao::~OcorrenciasApuracao()
{
    delete ui;
}
