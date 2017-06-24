#ifndef CADASTROCOLABORADOR_H
#define CADASTROCOLABORADOR_H

#include <QObject>
#include <QString>
#include <QDate>

// Inlcuindo clases manuais do projeto

class CadastroColaborador : public QObject
{
    Q_OBJECT
public:
    explicit CadastroColaborador(QObject *parent = nullptr);

    QString getCodigoDaEmpresa() const;
    void setCodigoDaEmpresa(const QString &value);

    QString getEmpresa() const;
    void setEmpresa(const QString &value);

    QString getCodigoDaFilial() const;
    void setCodigoDaFilial(const QString &value);

    QString getFilial() const;
    void setFilial(const QString &value);

    QString getCidadeRegiao() const;
    void setCidadeRegiao(const QString &value);

    QString getCNPJ() const;
    void setCNPJ(const QString &value);

    QString getMatricula() const;
    void setMatricula(const QString &value);

    QString getCPF() const;
    void setCPF(const QString &value);

    QString getPIS() const;
    void setPIS(const QString &value);

    QString getNome() const;
    void setNome(const QString &value);

    QDate getDataDeAdmissao() const;
    void setDataDeAdmissao(const QDate &value);

    QDate getDataDeNascimento() const;
    void setDataDeNascimento(const QDate &value);

    QString getCodigoDeVinculo() const;
    void setCodigoDeVinculo(const QString &value);

    QString getTabelaDeOrganograma() const;
    void setTabelaDeOrganograma(const QString &value);

    QString getNumeroDoLocal() const;
    void setNumeroDoLocal(const QString &value);

    QString getHierarquiaDeLocal() const;
    void setHierarquiaDeLocal(const QString &value);

    QString getSetor() const;
    void setSetor(const QString &value);

    QString getEstruturaDeCargos() const;
    void setEstruturaDeCargos(const QString &value);

    QString getCodigoDoCargo() const;
    void setCodigoDoCargo(const QString &value);

    QString getCargo() const;
    void setCargo(const QString &value);

    int getTipoDeSalario() const;
    void setTipoDeSalario(int value);

    double getSalario() const;
    void setSalario(double value);

private:
    QString CodigoDaEmpresa;
    QString Empresa;
    QString CodigoDaFilial;
    QString Filial;
    QString CidadeRegiao;
    QString CNPJ;
    QString Matricula;
    QString CPF;
    QString PIS;
    QString Nome;
    QDate DataDeAdmissao;
    QDate DataDeNascimento;
    QString CodigoDeVinculo;
    QString TabelaDeOrganograma;
    QString NumeroDoLocal;
    QString HierarquiaDeLocal;
    QString Setor;
    QString EstruturaDeCargos;
    QString CodigoDoCargo;
    QString Cargo;
    int TipoDeSalario;
    double Salario;

};

#endif // CADASTROCOLABORADOR_H
