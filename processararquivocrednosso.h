#ifndef PROCESSARARQUIVOCREDNOSSO_H
#define PROCESSARARQUIVOCREDNOSSO_H

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QByteArray>
#include <QDebug>
#include <QTableWidgetItem>
#include <QIODevice>
#include <QThread>

#include "controledao.h"
#include "exportararquivo.h"
#include "caixamensagemprogresso.h"
#include "cadastrocolaborador.h"


class CaixaMensagemProgresso;

namespace Ui {
class ProcessarArquivoCrednosso;
}

class ProcessarArquivoCrednosso : public QWidget
{
    Q_OBJECT
    QTableWidget *__tempTable;

public:
    explicit ProcessarArquivoCrednosso(QWidget *parent = 0);
    ~ProcessarArquivoCrednosso();

signals:
    void finishThread();
    void progressValue(int);
    void minimumProgressValue(int);
    void maximumProgressValue(int);
    void fecharCaixaDeMensagem();
    void obterUpdateDadosColaborador(int, int, int, int, bool);
    void obterUpdateDadosColaborador(UpdateDataTableColumm*);
    void cancelarProcesso();

private slots:
    void abrirArquivo();
    void converterArquivo();
    void converterArquivo(QString);
    void converterArquivoLayoutBasico();
    void converterArquivoCorteFerias();
    void converterArquivoCorteFeriasDAT();
    void converterArquivoCrednossoPlanoSaudeLinhas();
    void filtroItemTabela(QString);
    void exportarParaExcel();
    void caixaMensagemUsuario(QString);
    void cancelarOperacao();

    //Slots de atualizacao dos dados na tabela
    void atualizarDados();
    void limparDadosTempTable();
    void updateDadosArquivoLayoutBasico();
    void updateDadosArquivoLayoutBasico(UpdateDataTableColumm *);
    void updateDadosArquivoLayoutBasico(QMap<int, UpdateDataTableColumm *>);
    void updateDadosArquivoCrednossoFerias(QMap<int, ObjetoCadastroUpdateFile *>);
    void updateDadosArquivoCrednossoRetorno(QMap<int, ObjetoCadastroUpdateFile *>);
    void updateDadosArquivoCrednossoFeriasDAT(QMap<int, UpdateDataTableColumm *>);
    void updateDadosArquivoCrednossoMesCorte(QMap<int, ObjetoCadastroUpdateFile *>);
    void updateDadosArquivoCrednossoDemitidos(QMap<int, ObjetoCadastroUpdateFile *>);


private:
    Ui::ProcessarArquivoCrednosso *ui;
    QThread *threadInstancia;
    ControleDAO *controlador;
    CaixaMensagemProgresso *msg;
};

#endif // PROCESSARARQUIVOCREDNOSSO_H
