#include "detalhesretencao.h"
#include "ui_detalhesretencao.h"

DetalhesRetencao::DetalhesRetencao(
        QWidget *parent,
        QString responsavel,
        QString admitidos,
        QString demitidos,
        QString percentual,
        QString periodo)
    :
      QDialog(parent),
      responsavelSelecao(responsavel),
      numeroAdmitidos(admitidos),
      numeroDemitidos(demitidos),
      percentualRetido(percentual),
      periodoSelecionado(periodo),
      ui(new Ui::DetalhesRetencao)
{
    ui->setupUi(this); this->updateStatus();
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

void DetalhesRetencao::updateStatus()
{
    this->setWindowTitle(QString("Detalhes de ").append(this->getResponsavelSelecao()));

    QLocale local = this->locale();
    QString responsavel = this->getResponsavelSelecao();
    QString admitidos = this->getNumeroAdmitidos();
    QString demitidos = this->getNumeroDemitidos();
    QString percentual = this->getPercentualRetido();
    QString periodo = this->getPeriodoSelecionado();
    double p = local.toDouble(percentual);
    if(p > 10) { ui->percentualRetido->setStyleSheet(QString("color: red;")); } else { ui->percentualRetido->setStyleSheet(QString("color: green;")); }

    ui->campoResponsavelSelecao->setText(responsavel);
    ui->campoNumeroAdmitidos->setText(admitidos);
    ui->campoNumeroDemitidos->setText(demitidos);
    ui->campoPeriodoApuracao->setText(periodo);
    ui->percentualRetido->setText(percentual);
}
