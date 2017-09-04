#ifndef OBJETOCADASTROUPDATEFILE_H
#define OBJETOCADASTROUPDATEFILE_H

#include <QObject>

class ObjetoCadastroUpdateFile : public QObject
{
    Q_OBJECT
public:
    explicit ObjetoCadastroUpdateFile(QObject *parent = nullptr);

    int getChaveKey() const;
    void setChaveKey(int value);

    int getCodigoDaEmpresa() const;
    void setCodigoDaEmpresa(int value);

    int getMatricula() const;
    void setMatricula(int value);

    int getCPF() const;
    void setCPF(int value);

    QString getNome() const;
    void setNome(const QString &value);

    int getCodigoDaFilial() const;
    void setCodigoDaFilial(int value);

    QString getFilial() const;
    void setFilial(const QString &value);

    int getCodigoDaSituacao() const;
    void setCodigoDaSituacao(int value);

    int getCodidoDaCausa() const;
    void setCodidoDaCausa(int value);

signals:

public slots:

private:
    int chaveKey;
    int codigoDaEmpresa;
    int matricula;
    int CPF;
    QString Nome;
    int codigoDaFilial;
    QString filial;
    int codigoDaSituacao;
    int codidoDaCausa;
};

#endif // OBJETOCADASTROUPDATEFILE_H
