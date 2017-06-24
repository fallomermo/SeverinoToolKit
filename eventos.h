#ifndef EVENTOS_H
#define EVENTOS_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QMultiMap>
#include <QMapIterator>


// Inlcuindo clases manuais do projeto



class Eventos : public QObject
{
    Q_OBJECT
public:
    explicit Eventos(QObject *parent = 0);

    QString getID_Empresa() const;
    void setID_Empresa(const QString &value);

    QString getEmpresa() const;
    void setEmpresa(const QString &value);

    QString getID_Filial() const;
    void setID_Filial(const QString &value);

    QString getFilial() const;
    void setFilial(const QString &value);

    QString getCidadeRegiao() const;
    void setCidadeRegiao(const QString &value);

    int getCalculo() const;
    void setCalculo(int value);

    QDate getCompetencia() const;
    void setCompetencia(const QDate &value);

    QString getSetor() const;
    void setSetor(const QString &value);

    int getCodigoEvento() const;
    void setCodigoEvento(int value);

    QString getEvento() const;
    void setEvento(const QString &value);

    int getTipoEvento() const;
    void setTipoEvento(int value);

    double getValorEvento() const;
    void setValorEvento(double value);

    double getValorTerceiro() const;
    void setValorTerceiro(double value);

    double getValorGps() const;
    void setValorGps(double value);

    double getValorDeducao() const;
    void setValorDeducao(double value);

    QString getCnpj() const;
    void setCnpj(const QString &value);

    int getNumeroRegistros() const;
    void setNumeroRegistros(int value);

signals:

public slots:

private:
    QString ID_Empresa;
    QString Empresa;
    QString ID_Filial;
    QString Filial;
    QString Cnpj;
    QString CidadeRegiao;
    int Calculo;
    QDate Competencia;
    QString Setor;
    int CodigoEvento;
    QString Evento;
    int TipoEvento;
    int NumeroRegistros;
    double ValorEvento = 0.0;
    double ValorTerceiro = 0.0;
    double ValorGps = 0.0;
    double ValorDeducao = 0.0;
};

#endif // EVENTOS_H
