#ifndef ADMINISTRACAOGERAL_H
#define ADMINISTRACAOGERAL_H

#include <QWidget>
#include <QList>
#include <QDate>
#include <QModelIndex>

#include "controledao.h"
#include "usuarios.h"

namespace Ui {
class AdministracaoGeral;
}

class AdministracaoGeral : public QWidget
{
    Q_OBJECT

public:
    explicit AdministracaoGeral(QWidget *parent = 0, QList<Usuarios*> usr = QList<Usuarios*>());
    ~AdministracaoGeral();

    QList<Usuarios *> getUsuariosCadastrados() const;
    void setUsuariosCadastrados(const QList<Usuarios *> &value);

signals:
    void inserirUsuario(Usuarios*);
    void removerUsuario(Usuarios*);
    void alterarUsuario(Usuarios*);

private slots:
    void novoUsuario();
    void alterarUsuario();
    void excluirUsuario();
    void salvarUsuario();
    void cancelar();
    void statusBotoesAtivos();
    void statusBotoesInativos();
    void listarUsuarios(QList<Usuarios*>);
    void usuarioSelecionado(QModelIndex);
    void usuarioSelecionado(int, int);

private:
    Ui::AdministracaoGeral *ui;
    ControleDAO *controle;
    QThread *thread_DB;

    QList<Usuarios*> usuariosCadastrados;
};

#endif // ADMINISTRACAOGERAL_H
