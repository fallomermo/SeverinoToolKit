#include "cpocorrenciasapuracao.h"
#include "ui_cpocorrenciasapuracao.h"

CPOcorrenciasApuracao::CPOcorrenciasApuracao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPOcorrenciasApuracao)
{
    ui->setupUi(this);
}

CPOcorrenciasApuracao::~CPOcorrenciasApuracao()
{
    delete ui;
}

void CPOcorrenciasApuracao::processarRequisicao()
{

}

void CPOcorrenciasApuracao::atualizarTabela()
{

}
