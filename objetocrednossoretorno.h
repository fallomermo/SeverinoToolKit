#ifndef OBJETOCREDNOSSORETORNO_H
#define OBJETOCREDNOSSORETORNO_H

#include <QObject>

class ObjetoCrednossoRetorno : public QObject
{
    Q_OBJECT
public:
    explicit ObjetoCrednossoRetorno(QObject *parent = nullptr);

    int getCodigoDaEmpresa() const;
    void setCodigoDaEmpresa(int value);

    QString getCPF() const;
    void setCPF(const QString value);

    QString getEmpresa() const;
    void setEmpresa(const QString &value);

    int getCodigoDaFilial() const;
    void setCodigoDaFilial(int value);

    QString getFilial() const;
    void setFilial(const QString &value);

    QString getCNPJ() const;
    void setCNPJ(const QString &value);

    int getMatricula() const;
    void setMatricula(int value);

    QString getNome() const;
    void setNome(const QString &value);

    int getCodigoDoEvento() const;
    void setCodigoDoEvento(int value);

    QString getEvento() const;
    void setEvento(const QString &value);

    double getValor() const;
    void setValor(double value);

private:
    int CodigoDaEmpresa;
    QString CPF;
    QString Empresa;
    int CodigoDaFilial;
    QString Filial;
    QString CNPJ;
    int Matricula;
    QString Nome;
    int CodigoDoEvento;
    QString Evento;
    double Valor;
};

#endif // OBJETOCREDNOSSORETORNO_H
