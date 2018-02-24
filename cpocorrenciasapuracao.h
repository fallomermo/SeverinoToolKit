#ifndef CPOCORRENCIASAPURACAO_H
#define CPOCORRENCIASAPURACAO_H

#include <QWidget>
#include <QThread>

#include "controledao.h"
#include "exportararquivo.h"
#include "qcustomplot.h"


namespace Ui {
class CPOcorrenciasApuracao;
}

class CPOcorrenciasApuracao : public QWidget
{
    Q_OBJECT

public:
    explicit CPOcorrenciasApuracao(QWidget *parent = 0);
    ~CPOcorrenciasApuracao();

signals:
    void finishThead();
    void fecharCaixaMensagem();

private slots:
    void processarRequisicao();
    void atualizarTabela();



private:
    Ui::CPOcorrenciasApuracao *ui;
};

#endif // CPOCORRENCIASAPURACAO_H
