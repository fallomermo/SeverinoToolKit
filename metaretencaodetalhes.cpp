#include "metaretencaodetalhes.h"
#include "ui_metaretencaodetalhes.h"

MetaRetencaoDetalhes::MetaRetencaoDetalhes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MetaRetencaoDetalhes)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Detalhes de ").append(this->getResponsavelSelecao()));
    QLabel *statusResponsavel = new QLabel;
    QHBoxLayout *layGrid = new QHBoxLayout;

    ui->campoResponsavelSelecao->setText(this->getResponsavelSelecao());
    ui->campoNumeroAdmitidos->setText(this->getNumeroAdmitidos());
    ui->campoNumeroDemitidos->setText(this->getNumeroDemitido());
    ui->campoPercentualRetido->setText(this->getPercentualRetido());
    ui->campoPeriodoApuracao->setText(this->getPeriodoSelecionado());
    statusResponsavel->setPixmap(this->getImagemStatus());

    layGrid->addWidget(statusResponsavel);
    ui->groupBox_Status->setLayout(layGrid);
    statusResponsavel->show();
}

MetaRetencaoDetalhes::MetaRetencaoDetalhes(
        QWidget *parent,
        const QString &__res,
        const QString &__adm,
        const QString &__dem,
        const QString &__ret,
        const QString &__per)
    :
      QWidget(parent),
      responsavelSelecao(__res),
      numeroAdmitidos(__adm),
      numeroDemitido(__dem),
      percentualRetido(__ret),
      periodoSelecionado(__per),
      ui(new Ui::MetaRetencaoDetalhes)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Detalhes de ").append(this->getResponsavelSelecao()));
    QLabel *statusResponsavel = new QLabel;
    QHBoxLayout *layGrid = new QHBoxLayout;

    ui->campoResponsavelSelecao->setText(this->getResponsavelSelecao());
    ui->campoNumeroAdmitidos->setText(this->getNumeroAdmitidos());
    ui->campoNumeroDemitidos->setText(this->getNumeroDemitido());
    ui->campoPercentualRetido->setText(this->getPercentualRetido());
    ui->campoPeriodoApuracao->setText(this->getPeriodoSelecionado());
    statusResponsavel->setPixmap(this->getImagemStatus());

    layGrid->addWidget(statusResponsavel);
    ui->groupBox_Status->setLayout(layGrid);
    statusResponsavel->show();

}

MetaRetencaoDetalhes::~MetaRetencaoDetalhes()
{
    delete ui;
}

QMap<int, ObjetoRetencao *> MetaRetencaoDetalhes::getMapRetencao() const
{
    return mapRetencao;
}

void MetaRetencaoDetalhes::setMapRetencao(const QMap<int, ObjetoRetencao *> &value)
{
    mapRetencao = value;
}

QModelIndex MetaRetencaoDetalhes::getModelIndex() const
{
    return modelIndex;
}

void MetaRetencaoDetalhes::setModelIndex(const QModelIndex &value)
{
    modelIndex = value;
}

QString MetaRetencaoDetalhes::getResponsavelSelecao() const
{
    return responsavelSelecao;
}

void MetaRetencaoDetalhes::setResponsavelSelecao(const QString &value)
{
    responsavelSelecao = value;
}

QString MetaRetencaoDetalhes::getNumeroAdmitidos() const
{
    return numeroAdmitidos;
}

void MetaRetencaoDetalhes::setNumeroAdmitidos(const QString &value)
{
    numeroAdmitidos = value;
}

QString MetaRetencaoDetalhes::getNumeroDemitido() const
{
    return numeroDemitido;
}

void MetaRetencaoDetalhes::setNumeroDemitido(const QString &value)
{
    numeroDemitido = value;
}

QString MetaRetencaoDetalhes::getPercentualRetido() const
{
    return percentualRetido;
}

void MetaRetencaoDetalhes::setPercentualRetido(const QString &value)
{
    percentualRetido = value;
}

QString MetaRetencaoDetalhes::getPeriodoSelecionado() const
{
    return periodoSelecionado;
}

void MetaRetencaoDetalhes::setPeriodoSelecionado(const QString &value)
{
    periodoSelecionado = value;
}

QPixmap MetaRetencaoDetalhes::getImagemStatus() const
{
    return imagemStatus;
}

void MetaRetencaoDetalhes::setImagemStatus(const QPixmap &value)
{
    imagemStatus = value;
}
