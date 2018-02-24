#ifndef OCORRENCIASAPURACAO_H
#define OCORRENCIASAPURACAO_H

#include <QWidget>

namespace Ui {
class OcorrenciasApuracao;
}

class OcorrenciasApuracao : public QWidget
{
    Q_OBJECT

public:
    explicit OcorrenciasApuracao(QWidget *parent = 0);
    ~OcorrenciasApuracao();

private:
    Ui::OcorrenciasApuracao *ui;
};

#endif // OCORRENCIASAPURACAO_H
