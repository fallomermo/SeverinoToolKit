#include "caixamensagemprogresso.h"
#include "ui_caixamensagemprogresso.h"

CaixaMensagemProgresso::CaixaMensagemProgresso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaixaMensagemProgresso)
{
    ui->setupUi(this);

    connect(this, SIGNAL(closeJanela()), this, SLOT(fecharJanela()));
    connect(ui->botaoCancelar, SIGNAL(clicked(bool)), this, SLOT(cancelarProcessoAtual()));

    QMovie *movie = new QMovie(":/images/splash.gif");
    this->setMovie(movie);
    this->ocultarBarraProgresso(true);
    this->ocultarBotaoCancelar(true);
    ui->barraDeProgresso->setVisible(false);
    ui->barraDeProgresso->hide();
}

CaixaMensagemProgresso::~CaixaMensagemProgresso()
{
    delete ui;
}

void CaixaMensagemProgresso::setTituloJanela(const QString titulo)
{
    this->setWindowTitle( titulo );
}

void CaixaMensagemProgresso::setMinimumValue(int value)
{
    ui->barraDeProgresso->setVisible(true);
    ui->barraDeProgresso->show();
    ui->barraDeProgresso->setValue(value);
}

void CaixaMensagemProgresso::setMaximumValue(int value)
{
    ui->barraDeProgresso->setMaximum(value);
}

void CaixaMensagemProgresso::cancelarProcessoAtual()
{
    emit cancelarProcesso();
    this->close();
}

void CaixaMensagemProgresso::cancelarProcessoRequisicao()
{
    emit cancelarProcessoRequisicao();
}

void CaixaMensagemProgresso::setMovie(QMovie *movie)
{
    ui->labelImageFrame->setAttribute( Qt::WA_NoSystemBackground );
    ui->labelImageFrame->setMovie( movie );
    movie->start();
}

void CaixaMensagemProgresso::ocultarBarraProgresso(bool ok)
{
    ui->barraDeProgresso->setVisible(ok);
    ui->barraDeProgresso->show();
}

void CaixaMensagemProgresso::ocultarBotaoCancelar(bool ok)
{
    ui->botaoCancelar->setVisible(ok);
    ui->barraDeProgresso->hide();
}

void CaixaMensagemProgresso::setProgressValue(int value)
{
    ui->barraDeProgresso->setValue(value);
}

void CaixaMensagemProgresso::fecharJanela()
{
    this->close();
}
