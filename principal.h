#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QFile>
#include <QTimer>
#include <QTime>
#include <QMenu>
#include <QLabel>
#include <QVersionNumber>
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
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>


// Inlcuindo clases manuais do projeto
#include "planodecontas.h"
#include "controledao.h"
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "guiainssfolha.h"
#include "relacaocolaborador.h"
#include "metaretencao.h"
#include "homeinicio.h"
#include "metaretencaoestruturada.h"
#include "cpocorrenciasapuracao.h"
#include "higienizacaocrednosso.h"
#include "processararquivocrednosso.h"
#include "blissbeneficios.h"
#include "truncararquivostexto.h"
#include "downloadafd.h"
#include "gerenciamentorep.h"
#include "liquidofolha.h"
#include "sobre.h"
#include "controledeponto.h"
#include "administracaogeral.h"
#include "conversordearquivo.h"


namespace Ui {
class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = 0, QString u = nullptr);
    ~Principal();

    void aplicarDefinicoesGerais();

    QTime getTimeSession() const;
    void setTimeSession(const QTime &value);

    QMap<int, CadastroEmpresa *> getMapEmpresas() const;
    void setMapEmpresas(const QMap<int, CadastroEmpresa *> &value);

    QMap<int, CadastroFilial *> getMapFiliais() const;
    void setMapFiliais(const QMap<int, CadastroFilial *> &value);

    QString getUsuarioSessao() const;
    void setUsuarioSessao(const QString &value);

    bool getStatusConexaoSenior() const;
    void setStatusConexaoSenior(bool value);

signals:
    void finishThread();
    void obterConexaoBanco();
    void obterCadastroDeEmpresas();
    void obterCadastroDeFiliais();
    void trocarUsuario();
    void fecharSistema();

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
    void ocorrenciasApuracao();
    void relacaoColaborador();
    void higienizacaoCrednosso();
    void liquidoFolha();
    void ecoclinicRepasses();
    void downloadAhgoraAFD();
    void gerenciamentoREPs();
    void controleDePonto();
    void processarArquivosBliss();
    void processarArquivos();
    void truncarArquivos();
    void administracaoGeral();
    void infoSobreSistema();
    void conversorDeArquivo();
    void closeTab(int);
    void atualizarTema();
    void instalarTema();
    void updateTime();
    void mensagemRetornoUsuario(const QString, const QString);
    void mensagemControlador(QMessageBox&);
    void finishThreadBool(bool);

    void alterarUsuario();
    void usuarioAutenticado(QString);
    void updateStatusConnection();
    void encerrarSessao();
    void sairDoSistema();
    void finalizarThread();

private:
    Ui::Principal *ui;
    HomeInicio    *__homeInicio;
    PlanoDeContas *__planoContas;
    GuiaINSSFolha *__guiaInss;
    MetaRetencao  *__metaRetencao;
    MetaRetencaoEstruturada *__metaRetencaoEstruturada;
    CPOcorrenciasApuracao *__ocorrenciasApuracao;
    RelacaoColaborador *__relacaoColaboradores;
    HigienizacaoCrednosso *__crednosso;
    BlissBeneficios *__blissBeneficios;
    DownloadAFD *__downloadAhgoraAFD;
    GerenciamentoREP *__gerenciamentoREP;
    ProcessarArquivoCrednosso *__processarArquivos;
    TruncarArquivosTexto *__truncarArquivos;
    LiquidoFolha *__liquidoFolha;
    ControleDAO *controleFluxo;
    Sobre *__infoSobreSistema;
    ControleDePonto *__controleDePonto;
    AdministracaoGeral *__administracaoGeral;
    ConversorDeArquivo *__conversorDeArquivo;

    bool _flagHomeInicio;
    bool _flagPlanoContas;
    bool _flagPlanoSaude;
    bool _flagLiquidoFolha;
    bool _flagGuiaINSS;
    bool _flagEventosFolha;
    bool _flagMetaRetencaoNormal;
    bool _flagMetaRetencaoEstruturada;
    bool _flagOcorrenciasApuracao;
    bool _flagRelacaoColaborador;
    bool _flagHigienizacaoCrednosso;
    bool _flagProcessarBlissBeneficios;
    bool _flagDownloadAhgoraAFD;
    bool _flagEcoclinicRepasses;
    bool _flagProcessarArquivos;
    bool _flagTruncarArquivos;
    bool _flagInfoSobreSistema;
    bool _flagGerenciamentoREP;
    bool _flagControleDePonto;
    bool _flagAdministracaoGeral;
    bool _flagConversorDeArquivo;
    bool statusConexaoSenior;

    int _indexHomeInicio;
    int _indexPlanoContas;
    int _indexPlanoSaude;
    int _indexLiquidoFolha;
    int _indexGuiaINSS;
    int _indexEventosFolha;
    int _indexMetaRetencaoNormal;
    int _indexMetaRetencaoEstruturada;
    int _indexOcorrenciasApuracao;
    int _indexRelacaoColaborador;
    int _indexHigienizacaoCrednosso;
    int _indexProcessarBlissBeneficios;
    int _indexDownloadAhgoraAFD;
    int _indexEcoclinicRepasses;
    int _indexProcessarArquivos;
    int _indexTruncarArquivos;
    int _indexInfoSobreSistema;
    int _indexGerenciamentoREP;
    int _indexControleDePonto;
    int _indexAdministracaoGeral;
    int _indexConversorDeArquivo;

    QTimer *timer;
    QTimer *timerStatusSenior;
    QTime timeSession;
    QMovie *movieStatusSenior;
    QMap<int, CadastroEmpresa*> mapEmpresas;
    QMap<int, CadastroFilial*> mapFiliais;
    QString usuarioSessao;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // PRINCIPAL_H
