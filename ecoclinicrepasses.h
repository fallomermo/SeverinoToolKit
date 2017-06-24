#ifndef ECOCLINICREPASSES_H
#define ECOCLINICREPASSES_H

#include <QObject>
#include <QDate>


// Inlcuindo clases manuais do projeto


class EcoclinicRepasses : public QObject
{
    Q_OBJECT
public:
    explicit EcoclinicRepasses(QObject *parent = nullptr);

    QString getID_Empresa() const;
    void setID_Empresa(const QString &value);

    QString getEmpresa() const;
    void setEmpresa(const QString &value);

    QString getID_Filial() const;
    void setID_Filial(const QString &value);

    QString getFilial() const;
    void setFilial(const QString &value);

    QString getMatricula() const;
    void setMatricula(const QString &value);

    QString getCPF() const;
    void setCPF(const QString &value);

    QString getNome() const;
    void setNome(const QString &value);

    QString getCargo() const;
    void setCargo(const QString &value);

    QString getID_Sindicato() const;
    void setID_Sindicato(const QString &value);

    QString getSetor() const;
    void setSetor(const QString &value);

    QString getSexo() const;
    void setSexo(const QString &value);

    QString getDdd1() const;
    void setDdd1(const QString &value);

    QString getNumero1() const;
    void setNumero1(const QString &value);

    QString getDdd2() const;
    void setDdd2(const QString &value);

    QString getNumero2() const;
    void setNumero2(const QString &value);

    QString getRegistroGeralIdentidade() const;
    void setRegistroGeralIdentidade(const QString &value);

    QString getOrgaoEmissor() const;
    void setOrgaoEmissor(const QString &value);

    QString getUF_Emissor() const;
    void setUF_Emissor(const QString &value);

    QDate getDataEmissao() const;
    void setDataEmissao(const QDate &value);

    QDate getDataAdmissao() const;
    void setDataAdmissao(const QDate &value);

    QDate getCompetencia() const;
    void setCompetencia(const QDate &value);

    int getID_Evento() const;
    void setID_Evento(int value);

    QString getEvento() const;
    void setEvento(const QString &value);

    double getValor() const;
    void setValor(double value);

signals:

public slots:

private:
    QString ID_Empresa;
    QString Empresa;
    QString ID_Filial;
    QString Filial;
    QString Matricula;
    QString CPF;
    QString Nome;
    QString Cargo;
    QString ID_Sindicato;
    QString Setor;
    QString Sexo;
    QString Ddd1;
    QString Numero1;
    QString Ddd2;
    QString Numero2;
    QString RegistroGeralIdentidade;
    QString OrgaoEmissor;
    QString UF_Emissor;
    QDate DataEmissao;
    QDate DataAdmissao;
    QDate Competencia;
    int ID_Evento;
    QString Evento;
    double Valor;
};

#endif // ECOCLINICREPASSES_H
