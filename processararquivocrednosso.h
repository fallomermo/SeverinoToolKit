#ifndef PROCESSARARQUIVOCREDNOSSO_H
#define PROCESSARARQUIVOCREDNOSSO_H

#include <QWidget>

namespace Ui {
class ProcessarArquivoCrednosso;
}

class ProcessarArquivoCrednosso : public QWidget
{
    Q_OBJECT

public:
    explicit ProcessarArquivoCrednosso(QWidget *parent = 0);
    ~ProcessarArquivoCrednosso();

private:
    Ui::ProcessarArquivoCrednosso *ui;
};

#endif // PROCESSARARQUIVOCREDNOSSO_H
