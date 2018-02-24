#ifndef CONVERSORDEARQUIVO_H
#define CONVERSORDEARQUIVO_H

#include <QWidget>
#include <QThread>

#include "controledao.h"
#include "eventosimportadosobj.h"

namespace Ui {
class ConversorDeArquivo;
}

class ConversorDeArquivo : public QWidget
{
    Q_OBJECT
public:
    explicit ConversorDeArquivo(QWidget *parent = 0);
    ~ConversorDeArquivo();

signals:
    void obterEventosImportados(QString, int);
    void finishThread();
    void fecharCaixaMensagem();

private slots:
    void analisarEventosImportados();
    void carregarEventosImportados(QList<EventosImportadosOBJ*>);

private:
    Ui::ConversorDeArquivo *ui;
    QThread *processo;
    ControleDAO *controle;
};

#endif // CONVERSORDEARQUIVO_H
