#ifndef OBJETORETENCAO_H
#define OBJETORETENCAO_H

#include <QObject>
#include <QDate>

class ObjetoRetencao : public QObject
{
    Q_OBJECT
public:
    explicit ObjetoRetencao(QObject *parent = nullptr);

    int getCadastro() const;
    void setCadastro(int value);

    QString getNome() const;
    void setNome(const QString &value);

    QDate getDemissao() const;
    void setDemissao(const QDate &value);

    QDate getAdmissao() const;
    void setAdmissao(const QDate &value);

    QString getCargo() const;
    void setCargo(const QString &value);

    QString getSetor() const;
    void setSetor(const QString &value);

    int getCodigoDaCausa() const;
    void setCodigoDaCausa(int value);

    QString getCausa() const;
    void setCausa(const QString &value);

    int getCodigoDaEmpresa() const;
    void setCodigoDaEmpresa(int value);

    int getCodigoDaFilial() const;
    void setCodigoDaFilial(int value);

    QString getFilial() const;
    void setFilial(const QString &value);

    QString getRecrutador() const;
    void setRecrutador(const QString &value);

    int getAdmitidos() const;
    void setAdmitidos(int value);

    int getDemitidos() const;
    void setDemitidos(int value);

signals:

public slots:

private:
    int cadastro;
    QString nome;
    QDate demissao;
    QDate admissao;
    QString cargo;
    QString setor;
    int codigoDaCausa;
    QString causa;
    int codigoDaEmpresa;
    int codigoDaFilial;
    QString filial;
    QString recrutador;
    int admitidos;
    int demitidos;


};

#endif // OBJETORETENCAO_H
