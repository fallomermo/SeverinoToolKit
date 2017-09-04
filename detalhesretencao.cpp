#include "detalhesretencao.h"
#include "ui_detalhesretencao.h"

DetalhesRetencao::DetalhesRetencao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetalhesRetencao)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Detalhes de ").append(this->getResponsavelSelecao()));
    this->periodoSelecionado = "";

    ui->campoResponsavelSelecao->setText(this->getResponsavelSelecao());
    ui->campoNumeroAdmitidos->setText(this->getNumeroAdmitidos());
    ui->campoNumeroDemitidos->setText(this->getNumeroDemitidos());
    ui->campoPercentualRetido->setText(this->getPercentualRetido());
    ui->campoPeriodoApuracao->setText(this->getPeriodoSelecionado());
    QPixmap __pixmap(this->getImagemStatus());
    ui->statusRecrutador->setPixmap(__pixmap);
}

DetalhesRetencao::~DetalhesRetencao()
{
    delete ui;
}

QMap<int, ObjetoRetencao *> DetalhesRetencao::getMapRetencao() const
{
    return mapRetencao;
}

void DetalhesRetencao::setMapRetencao(const QMap<int, ObjetoRetencao *> &value)
{
    mapRetencao = value;
}

QModelIndex DetalhesRetencao::getModelIndex() const
{
    return modelIndex;
}

void DetalhesRetencao::setModelIndex(const QModelIndex &value)
{
    modelIndex = value;
}

QString DetalhesRetencao::getResponsavelSelecao() const
{
    return responsavelSelecao;
}

void DetalhesRetencao::setResponsavelSelecao(const QString &value)
{
    responsavelSelecao = value;
}

QString DetalhesRetencao::getNumeroAdmitidos() const
{
    return numeroAdmitidos;
}

void DetalhesRetencao::setNumeroAdmitidos(const QString &value)
{
    numeroAdmitidos = value;
}

QString DetalhesRetencao::getNumeroDemitidos() const
{
    return numeroDemitidos;
}

void DetalhesRetencao::setNumeroDemitidos(const QString &value)
{
    numeroDemitidos = value;
}

QString DetalhesRetencao::getPercentualRetido() const
{
    return percentualRetido;
}

void DetalhesRetencao::setPercentualRetido(const QString &value)
{
    percentualRetido = value;
}

QString DetalhesRetencao::getPeriodoSelecionado() const
{
    return periodoSelecionado;
}

void DetalhesRetencao::setPeriodoSelecionado(const QString &value)
{
    periodoSelecionado = value;
}

QPixmap DetalhesRetencao::getImagemStatus() const
{
    return imagemStatus;
}

void DetalhesRetencao::setImagemStatus(const QPixmap &value)
{
    imagemStatus = value;
}
