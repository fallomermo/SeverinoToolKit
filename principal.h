#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QFile>
#include <QTimer>
#include <QTime>
#include <QMenu>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QLocale>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
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

private slots:
    void planoContas();
    void planoSaude();
    void guiaInssFolha();
    void relacaoColaborador();
    void closeTab(int);
    void atualizarTema();
    void instalarTema();
    void updateTime();
    void mensagemRetornoUsuario(QString);

private:
    Ui::Principal *ui;
    PlanoDeContas *pc;
    PlanoSaude    *ps;
    GuiaINSSFolha *guiaInss;
    RelacaoColaborador *relCol;
    ControleDAO *ctrl;

    bool _flagPlanoContas;
    bool _flagPlanoSaude;
    bool _flagLiquidoFolha;
    bool _flagGuiaINSS;
    bool _flagEventosFolha;
    bool _flagMetaRetencao;
    bool _flagRelacaoColaborador;

    int _indexPlanoContas;
    int _indexPlanoSaude;
    int _indexLiquidoFolha;
    int _indexGuiaINSS;
    int _indexEventosFolha;
    int _indexMetaRetencao;
    int _indexRelacaoColaborador;

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
