#ifndef EVENTOSIMPORTADOSOBJ_H
#define EVENTOSIMPORTADOSOBJ_H

#include <QObject>

class EventosImportadosOBJ : public QObject
{
    Q_OBJECT
public:
    explicit EventosImportadosOBJ(QObject *parent = nullptr);

    QString getEmpresa() const;
    void setEmpresa(const QString &value);

    QString getFilial() const;
    void setFilial(const QString &value);

    QString getMatricula() const;
    void setMatricula(const QString &value);

    QString getNome() const;
    void setNome(const QString &value);

    QString getCpf() const;
    void setCpf(const QString &value);

    QString getSindicato() const;
    void setSindicato(const QString &value);

    QString getEvento() const;
    void setEvento(const QString &value);

    QString getReferencia() const;
    void setReferencia(const QString &value);

    QString getValor() const;
    void setValor(const QString &value);

    QString getOrigem() const;
    void setOrigem(const QString &value);

private:
    QString empresa;
    QString filial;
    QString matricula;
    QString nome;
    QString cpf;
    QString sindicato;
    QString evento;
    QString referencia;
    QString valor;
    QString origem;
};

#endif // EVENTOSIMPORTADOSOBJ_H
