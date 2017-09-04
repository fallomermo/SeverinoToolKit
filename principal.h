#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QFile>
#include <QTimer>
#include <QTime>
#include <QMenu>
#include <QLabel>
#include <QDebug>
#include <QThread>
#include <QWidget>
#include <QLocale>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
#include <QKeySequence>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QGraphicsDropShadowEffect>


// Inlcuindo clases manuais do projeto
#include "planodecontas.h"
#include "controledao.h"
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "planosaude.h"
#include "guiainssfolha.h"
#include "relacaocolaborador.h"
#include "metaretencao.h"
#include "homeinicio.h"
#include "metaretencaoestruturada.h"
#include "higienizacaocrednosso.h"
#include "processararquivocrednosso.h"
#include "blissbeneficios.h"
#include "truncararquivostexto.h"
#include "downloadahgoraafd.h"


namespace Ui {
class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = 0);
    ~Principal();

    void aplicarDefinicoesGerais();

    QTime getTimeSession() const;
    void setTimeSession(const QTime &value);

    QMap<int, CadastroEmpresa *> getMapEmpresas() const;
    void setMapEmpresas(const QMap<int, CadastroEmpresa *> &value);

    QMap<int, CadastroFilial *> getMapFiliais() const;
    void setMapFiliais(const QMap<int, CadastroFilial *> &value);

signals:
    void finishThread();
    void obterConexaoBanco();
    void obterCadastroDeEmpresas();
    void obterCadastroDeFiliais();

private slots:
    void homeInicio();
    void statusConexao(bool);
    void definirCadastrosDeEmpresas(const QMap<int, CadastroEmpresa *>);
    void definirCadastrosDeFiliais(const QMap<int, CadastroFilial*>);
    void planoContas();
    void planoSaude();
    void guiaInssFolha();
    void metaRetencaoNormal();
    void metaRetencaoEstruturada();
    void relacaoColaborador();
    void higienizacaoCrednosso();
    void downloadAhgoraAFD();
    void processarArquivosBliss();
    void processarArquivos();
    void truncarArquivos();
    void closeTab(int);
    void atualizarTema();
    void instalarTema();
    void updateTime();
    void mensagemRetornoUsuario(QString);
    void mensagemControlador(QMessageBox&);
    void finishThreadBool(bool);

private:
    Ui::Principal *ui;
    HomeInicio    *__homeInicio;
    PlanoDeContas *__planoContas;
    PlanoSaude    *__planoSaude;
    GuiaINSSFolha *__guiaInss;
    MetaRetencao  *__metaRetencao;
    MetaRetencaoEstruturada *__metaRetencaoEstruturada;
    RelacaoColaborador *__relacaoColaboradores;
    HigienizacaoCrednosso *__crednosso;
    BlissBeneficios *__blissBeneficios;
    DownloadAhgoraAFD *__downloadAhgoraAFD;
    ProcessarArquivoCrednosso *__processarArquivos;
    TruncarArquivosTexto *__truncarArquivos;
    ControleDAO *controleFluxo;

    bool _flagHomeInicio;
    bool _flagPlanoContas;
    bool _flagPlanoSaude;
    bool _flagLiquidoFolha;
    bool _flagGuiaINSS;
    bool _flagEventosFolha;
    bool _flagMetaRetencaoNormal;
    bool _flagMetaRetencaoEstruturada;
    bool _flagRelacaoColaborador;
    bool _flagHigienizacaoCrednosso;
    bool _flagProcessarBlissBeneficios;
    bool _flagDownloadAhgoraAFD;
    bool _flagProcessarArquivos;
    bool _flagTruncarArquivos;

    int _indexHomeInicio;
    int _indexPlanoContas;
    int _indexPlanoSaude;
    int _indexLiquidoFolha;
    int _indexGuiaINSS;
    int _indexEventosFolha;
    int _indexMetaRetencaoNormal;
    int _indexMetaRetencaoEstruturada;
    int _indexRelacaoColaborador;
    int _indexHigienizacaoCrednosso;
    int _indexProcessarBlissBeneficios;
    int _indexDownloadAhgoraAFD;
    int _indexProcessarArquivos;
    int _indexTruncarArquivos;

    QTimer *timer;
    QTime timeSession;
    QMap<int, CadastroEmpresa*> mapEmpresas;
    QMap<int, CadastroFilial*> mapFiliais;

    QLocale local;

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // PRINCIPAL_H
