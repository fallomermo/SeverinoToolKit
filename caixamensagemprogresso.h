#ifndef CAIXAMENSAGEMPROGRESSO_H
#define CAIXAMENSAGEMPROGRESSO_H

#include <QDialog>
#include <QMovie>
#include <QWidget>

namespace Ui {
class CaixaMensagemProgresso;
}

class CaixaMensagemProgresso : public QDialog
{
    Q_OBJECT

public:
    explicit CaixaMensagemProgresso(QWidget *parent = 0);
    ~CaixaMensagemProgresso();

    void setTituloJanela(const QString);
    void setMovie(QMovie*);
    void ocultarBarraProgresso(bool);
    void ocultarBotaoCancelar(bool);

signals:
    void closeJanela();
    void cancelarProcesso();

public slots:
    void setProgressValue(int);
    void setMinimumValue(int);
    void setMaximumValue(int);
    void cancelarProcessoAtual();
    void cancelarProcessoRequisicao();
    void fecharJanela();

private:
    Ui::CaixaMensagemProgresso *ui;
};

#endif // CAIXAMENSAGEMPROGRESSO_H
