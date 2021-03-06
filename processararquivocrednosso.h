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

private slots:
    void abrirArquivo();
    void converterArquivo();
    void converterArquivo(QString);
    void converterArquivoCorteFerias();
    void converterArquivoCorteFeriasDAT();
    void converterArquivoGarantiaCredinosso();
    void atualizarDadosCorteFeriasDATSenior();
    void converterArquivoCrednossoPlanoSaudeLinhas();
    void filtroItemTabela(QString);
    void exportarParaExcel();
    void caixaMensagemUsuario(QString);

    //Slots de atualizacao dos dados na tabela
    void atualizarDados();
    void limparDadosTempTable();
    void updateDadosArquivoCrednossoFerias(QMap<int, ObjetoCadastroUpdateFile *>);
    void updateDadosArquivoCrednossoRetorno(QMap<int, ObjetoCadastroUpdateFile *>);
    void updateDadosArquivoCrednossoGarantia();
    void updateDadosArquivoCrednossoGarantia(QMap<int, UpdateDataTableColumm *>);
    void updateDadosArquivoCrednossoFeriasDAT(QMap<int, UpdateDataTableColumm *>);
    void updateDadosArquivoCrednossoMesCorte(QMap<int, ObjetoCadastroUpdateFile *>);
    void updateDadosArquivoCrednossoDemitidos(QMap<int, ObjetoCadastroUpdateFile *>);


private:
    Ui::ProcessarArquivoCrednosso *ui;
};

#endif // PROCESSARARQUIVOCREDNOSSO_H
