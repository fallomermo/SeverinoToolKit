#include "bancodedados.h"

BancoDeDados::BancoDeDados(QObject *parent) : QObject(parent) {}

bool BancoDeDados::abrirConexao()
{
    QString connectString;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    connectString.append("DRIVER={SQL Server};");
    connectString.append("Server=192.168.6.4,1433;");
    connectString.append("DATABASE=vetorh;");
    connectString.append("UID=sa;");
    connectString.append("PWD=rhsenior123;");
    connectString.append("WSID=;");
    db.setDatabaseName(connectString);

    if(!db.open()) {
        emit messagemRetorno(db.lastError().text());
        qDebug() << db.lastError().text();
        qDebug() << db.drivers();
        return false;
    } else {
        return true;
    }
}

QMap<int, CadastroEmpresa *> BancoDeDados::getEmpresas()
{
    QMap<int, CadastroEmpresa *> m;
    QSqlQuery consulta;
    QString comando = QString("SELECT NUMEMP, NOMEMP FROM R030EMP WHERE NUMEMP > 1000 ORDER BY NUMEMP");
    consulta.prepare(comando);
    if(consulta.exec()){
        consulta.setForwardOnly(true);
        while (consulta.next()) {
            _cemp = new CadastroEmpresa();
            _cemp->setID_Empresa(QString::number(consulta.value("NUMEMP").toInt(nullptr)));
            _cemp->setEmpresa(consulta.value("NOMEMP").toString());
            m.insert(consulta.value("NUMEMP").toInt(nullptr),_cemp);
        }
    } else {
        emit messagemRetorno(consulta.lastError().text());
    }
    return m;
}

QMap<int, CadastroFilial *> BancoDeDados::getFiliais()
{
    QMap<int, CadastroFilial *> m;
    QSqlQuery consulta;
    QString comando = QString("SELECT NUMEMP, CODFIL, NOMFIL, NUMCGC FROM R030FIL WHERE NUMEMP > 1000 ORDER BY NUMEMP, CODFIL");
    consulta.prepare(comando);
    if(consulta.exec()){
        consulta.setForwardOnly(true);
        int indice = 0;
        while (consulta.next()) {
            indice++;
            _cfil = new CadastroFilial();
            _cfil->setID_Empresa(QString::number(consulta.value("NUMEMP").toInt(nullptr)));
            _cfil->setID_Filial(QString::number(consulta.value("CODFIL").toInt(nullptr)));
            _cfil->setFilial(consulta.value("NOMFIL").toString());
            m.insert(indice,_cfil);
        }
    } else {
        emit messagemRetorno(consulta.lastError().text());
    }
    return m;
}

QMap<int, Eventos *> BancoDeDados::getPlanoContas(QString __idEmpresa, QString __idFilial, QString __IniComp, QString __FimComp, int __tipCal)
{
    QMap<int, Eventos *> __tempMap;
    QString argumentos;
    if(__idEmpresa.isEmpty()) {
        argumentos.append(" > 1000 ");
    } else {
        argumentos.append(QString(" IN ( %0 ) ").arg(__idEmpresa));

        if(!__idFilial.isEmpty())
            argumentos.append(QString(" AND codfil IN ( %0 ) ").arg(__idFilial));
    }
    argumentos.append(QString(" AND inicmp between ' %0 ' AND ' %1 ' AND tipcal = %2 ").arg(__IniComp).arg(__FimComp).arg(QString::number(__tipCal)));

    QString comando = QString(" SELECT numemp, "
                              "        razsoc, "
                              "        codfil, "
                              "        nomfil, "
                              "        numcgc, "
                              "        nomcid, "
                              "        codcal, "
                              "        inicmp, "
                              "        tipcal, "
                              "        nomloc, "
                              "        codeve, "
                              "        deseve, "
                              "        tipeve, "
                              "        valeve "
                              " FROM   view_planocontas_agregado "
                              " WHERE  numemp %0 ").arg(argumentos);
    QSqlQuery consulta;
    consulta.setForwardOnly(true);
    consulta.prepare(comando);

    if(!consulta.exec()) {
        emit messagemRetorno(consulta.lastError().text());
    } else {
        int pos = 0;
        while (consulta.next()) {
            pos++;
            evt = new Eventos();
            this->statusProgressoAtual(pos,evt->getNumeroRegistros());
            evt->setID_Empresa(QVariant( consulta.value( "numemp" ) ).toString());
            evt->setEmpresa(QVariant( consulta.value( "razsoc" ) ).toString());
            evt->setID_Filial(QVariant( consulta.value( "codfil" ) ).toString());
            evt->setFilial(QVariant( consulta.value( "nomfil" ) ).toString());
            evt->setCnpj(QVariant( consulta.value("numcgc")).toString());
            evt->setCidadeRegiao(QVariant( consulta.value( "nomcid" ) ).toString());
            evt->setCalculo(QVariant( consulta.value( "codcal" ) ).toInt(0));
            evt->setCompetencia(QVariant( consulta.value( "inicmp" ) ).toDate());
            evt->setTipoDeCalculo(QVariant( consulta.value( "tipcal" ) ).toInt(0));
            evt->setSetor(QVariant( consulta.value( "nomloc" ) ).toString());
            evt->setCodigoEvento(QVariant( consulta.value( "codeve" ) ).toInt(0));
            evt->setEvento(QVariant( consulta.value("deseve") ).toString());
            evt->setTipoEvento(QVariant( consulta.value("tipeve") ).toInt(0));
            evt->setValorEvento(QVariant( consulta.value("valeve") ).toDouble(0));
            evt->setValorTerceiro(0);
            evt->setValorGps(0);
            evt->setValorDeducao(0);
            __tempMap.insert(pos, evt);
        }
    }
    return __tempMap;
}

QMap<int, EcoclinicRepasses *> BancoDeDados::getPlanoSaude(QString __dataInicial, QString __dataFinal, int r)
{
    QMap<int, EcoclinicRepasses *> __tempMap;

    QString __tipoRelatorio;
    if(r == 0)
        __tipoRelatorio = QString(" AND VER.codeve IN ( 1692, 1693, 1694, 1696, 1785, 1804, 1807 ) ");
    else
        __tipoRelatorio = QString(" AND VER.codeve IN ( 1771 ) ");

    QString comando = QString(" "
                              " SELECT FUN.numemp                            AS CODEMPRESA, "
                              "        FIL.razsoc                            AS EMPRESA, "
                              "        FUN.codfil                            AS CODFIL, "
                              "        FIL.nomfil                            AS FILIAL, "
                              "        FUN.numcad                            AS MATRICULA, "
                              "        FUN.numcpf                            AS CPF, "
                              "        FUN.nomfun                            AS NOME, "
                              "        CAR.titcar                            AS CARGO, "
                              "        HSI.codsin                            AS SINDICATO, "
                              "        ORN.nomloc                            AS SETOR, "
                              "        FUN.tipsex                            AS SEXO, "
                              "        CPL.dddtel                            AS DDD1, "
                              "        CPL.numtel                            AS NUMERO1, "
                              "        CPL.nmddd2                            AS DDD2, "
                              "        CPL.nmtel2                            AS NUMERO2, "
                              "        CPL.numcid                            AS RG, "
                              "        CPL.emicid                            AS ORGAOEMISSOR, "
                              "        CPL.estcid                            AS UFEMISSOR, "
                              "        CPL.dexcid                            AS DATAEMISSAO, "
                              "        FUN.datadm                            AS ADMISSAO, "
                              "        CAL.inicmp                            AS COMPETENCIA, "
                              "        VER.codeve                            AS CODEVENTO, "
                              "        EVC.deseve                            AS EVENTO, "
                              "        VER.valeve                            AS VALOR "
                              " FROM   r046ver VER (nolock) "
                              "        INNER JOIN r034fun FUN (nolock) "
                              "                ON ( FUN.numemp = VER.numemp "
                              "                     AND FUN.numcad = VER.numcad ) "
                              "        INNER JOIN r034cpl CPL (nolock) "
                              "                ON ( CPL.numemp = VER.numemp "
                              "                     AND CPL.numcad = VER.numcad ) "
                              "        INNER JOIN r044cal CAL (nolock) "
                              "                ON ( CAL.numemp = VER.numemp "
                              "                     AND CAL.codcal = VER.codcal ) "
                              "        INNER JOIN r008evc EVC (nolock) "
                              "                ON ( EVC.codtab = VER.tabeve "
                              "                     AND EVC.codeve = VER.codeve ) "
                              "        INNER JOIN r024car CAR (nolock) "
                              "                ON ( CAR.estcar = FUN.estcar "
                              "                     AND CAR.codcar = FUN.codcar ) "
                              "        INNER JOIN r030fil FIL (nolock) "
                              "                ON ( FIL.numemp = VER.numemp "
                              "                     AND FIL.codfil = FUN.codfil ) "
                              "        INNER JOIN r038hsi HSI (nolock) "
                              "                ON ( HSI.numemp = VER.numemp "
                              "                     AND HSI.numcad = VER.numcad "
                              "                     AND HSI.datalt = (SELECT Max(TB1.datalt) "
                              "                                       FROM   r038hsi TB1 (nolock) "
                              "                                       WHERE  TB1.numemp = VER.numemp "
                              "                                              AND TB1.numcad = VER.numcad "
                              "                                              AND TB1.datalt <= Dateadd(day, -1, Dateadd(month, Datediff( month, 0, CAL.inicmp) + 1, 0))) ) "
                              "        INNER JOIN r016orn ORN (nolock) "
                              "                ON ( ORN.taborg = FUN.taborg "
                              "                     AND ORN.numloc = FUN.numloc ) "
                              " WHERE  VER.numemp > 1000 "
                              "        AND CAL.tipcal = 11 "
                              "        AND CAL.inicmp BETWEEN ' %0 ' AND ' %1 ' "
                              "        AND HSI.codsin IN ( 4, 6, 24, 26 ) "
                              "        %2 "
                              " ORDER  BY VER.numemp, "
                              "           FIL.codfil, "
                              "           VER.numcad ").arg(__dataInicial).arg(__dataFinal).arg(__tipoRelatorio);
    QSqlQuery consulta;
    consulta.prepare(comando);
    if(!consulta.exec()) {
        emit messagemRetorno(consulta.lastError().text());
        return __tempMap;
    } else {
        consulta.setForwardOnly(true);
        int pos = 0;
        while (consulta.next()) {
            pos++;
            EcoclinicRepasses *repasses = new EcoclinicRepasses;
            repasses->setID_Empresa(QVariant( consulta.value( "CODEMPRESA" ) ).toString());
            repasses->setEmpresa(QVariant( consulta.value( "EMPRESA" ) ).toString());
            repasses->setID_Filial(QVariant( consulta.value( "CODFIL" ) ).toString());
            repasses->setFilial(QVariant( consulta.value( "FILIAL" ) ).toString());
            repasses->setMatricula(QVariant( consulta.value( "MATRICULA" ) ).toString());
            repasses->setCPF(QVariant( consulta.value( "CPF" ) ).toString());
            repasses->setNome(QVariant( consulta.value( "NOME" ) ).toString());
            repasses->setCargo(QVariant( consulta.value( "CARGO" ) ).toString());
            repasses->setID_Sindicato(QVariant( consulta.value( "SINDICATO" ) ).toString());
            repasses->setSetor(QVariant( consulta.value( "SETOR" ) ).toString());
            repasses->setSexo(QVariant( consulta.value( "SEXO" ) ).toString());
            repasses->setDdd1(QVariant( consulta.value( "DDD1" ) ).toString());
            repasses->setNumero1(QVariant( consulta.value( "NUMERO1" ) ).toString());
            repasses->setDdd2(QVariant( consulta.value( "DDD2" ) ).toString());
            repasses->setNumero2(QVariant( consulta.value( "NUMERO2" ) ).toString());
            repasses->setRegistroGeralIdentidade(QVariant( consulta.value( "RG" ) ).toString());
            repasses->setOrgaoEmissor(QVariant( consulta.value( "ORGAOEMISSOR" ) ).toString());
            repasses->setUF_Emissor(QVariant( consulta.value( "UFEMISSOR" ) ).toString());
            repasses->setDataEmissao(QVariant( consulta.value( "DATAEMISSAO" ) ).toDate());
            repasses->setDataAdmissao(QVariant( consulta.value( "ADMISSAO" ) ).toDate());
            repasses->setCompetencia(QVariant( consulta.value( "COMPETENCIA" ) ).toDate());
            repasses->setID_Evento(QVariant( consulta.value( "CODEVENTO" ) ).toInt(nullptr));
            repasses->setEvento(QVariant( consulta.value( "EVENTO" ) ).toString());
            repasses->setValor(QVariant( consulta.value( "VALOR" ) ).toDouble(nullptr));
            __tempMap.insert(pos, repasses);
        }
    }
    return __tempMap;
}

QMap<int, Eventos *> BancoDeDados::getGuiaINSS(QString __idEmpresa, QString __idFilial, QString __IniComp, QString __FimComp, int __tipoCalculo)
{
    setSaldoRateioNegativo(0.0);
    if(__tipoCalculo == 14) {
        __IniComp.replace('-','/');
        __FimComp.replace('-','/');
        QDate __nIni( QDate::fromString(__IniComp,"yyyy/MM/dd") );
        QDate __nFim( QDate::fromString(__FimComp,"yyyy/MM/dd") );
        QDate __n_ini(__nIni.year(), __nIni.month()-1, __nIni.day());
        QDate __n_fim(__nFim.year(), __nFim.month()-1, __nFim.day());
        __IniComp = __n_ini.toString(Qt::ISODate);
        __FimComp = __n_fim.toString(Qt::ISODate);
    }

    QMap<int, Eventos *> __tempMap;
    int pos = 0;
    QString argumentos;
    if(__idEmpresa.isEmpty())
        argumentos.append(" > 1000 ");
    else
        argumentos.append(QString(" IN ( %0 ) ").arg(__idEmpresa));

    if(!__idFilial.isEmpty())
        argumentos.append(QString(" AND gps.codfil IN ( %0 ) ").arg(__idFilial));

    argumentos.append(QString(" AND gps.cmpref BETWEEN ' %0 ' AND ' %1 ' AND gps.tipcal = %2 ")
                      .arg(__IniComp).arg(__FimComp).arg(QString::number(__tipoCalculo)));
    QString comando = QString(" "
                              " SELECT gps.numemp, "
                              " fil.razsoc, "
                              " gps.codfil, "
                              " fil.nomfil, "
                              " fil.numcgc, "
                              " cid.nomcid, "
                              " gps.codcal, "
                              " gps.cmpref, "
                              " gps.tipcal, "
                              " orn.nomloc, "
                              " Sum( gps.valter ) valter, "
                              " Sum( ( gps.valseg + gps.acitra + gps.valemp ) ) valgps, "
                              " Sum( gps.valded ) valded "
                              " FROM view_gps_rat gps "
                              " INNER JOIN r030fil fil "
                              " ON fil.numemp = gps.numemp "
                              " AND fil.codfil = gps.codfil "
                              " INNER JOIN r016orn orn "
                              " ON orn.taborg = 101 "
                              " AND orn.numloc = gps.numloc "
                              " INNER JOIN r074cid cid "
                              " ON cid.estcid = fil.codest "
                              " AND cid.codcid = fil.codcid "
                              " WHERE  gps.numemp %0 "
                              " GROUP  BY gps.numemp, "
                              " fil.razsoc, "
                              " gps.codfil, "
                              " fil.nomfil, "
                              " fil.numcgc, "
                              " cid.nomcid, "
                              " gps.codcal, "
                              " gps.cmpref, "
                              " gps.tipcal, "
                              " orn.nomloc ")
            .arg(argumentos);
    QSqlQuery consulta;
    QMap<QString, QMap<int, Eventos*>> __mapGlobal;
    consulta.prepare(comando);
    QString __cnpjAtu;
    QString __cnpjOld;
    if(!consulta.exec()) {
        emit messagemRetorno(consulta.lastError().text());
    } else {
        consulta.setForwardOnly(true);
        while (consulta.next()) {
            pos++;
            evt = new Eventos();
            this->statusProgressoAtual(pos,evt->getNumeroRegistros());
            evt->setID_Empresa(QVariant( consulta.value( "numemp" ) ).toString());
            evt->setEmpresa(QVariant( consulta.value( "razsoc" ) ).toString());
            evt->setID_Filial(QVariant( consulta.value( "codfil" ) ).toString());
            evt->setFilial(QVariant( consulta.value( "nomfil" ) ).toString());
            evt->setCnpj(QVariant( consulta.value("numcgc") ).toString());
            evt->setCidadeRegiao(QVariant( consulta.value( "nomcid" ) ).toString());
            evt->setCalculo(QVariant( consulta.value( "codcal" ) ).toInt(0));
            evt->setCompetencia(QVariant( consulta.value( "cmpref" ) ).toDate());
            evt->setTipoDeCalculo( 14 );
            evt->setSetor(QVariant( consulta.value( "nomloc" ) ).toString());
            evt->setCodigoEvento(QVariant( 9998 ).toInt(0));
            evt->setEvento(QVariant( QString("Guia INSS Folha") ).toString());
            evt->setTipoEvento(QVariant( 1 ).toInt(0));
            evt->setValorEvento(QVariant( 0.0 ).toDouble(nullptr));
            evt->setValorTerceiro(QVariant( consulta.value("valter") ).toDouble(nullptr));
            evt->setValorGps(QVariant( consulta.value("valgps") ).toDouble(nullptr));
            evt->setValorDeducao(QVariant( consulta.value("valded") ).toDouble(nullptr));
            __cnpjAtu = evt->getCnpj();

            if(!__cnpjAtu.contains(__cnpjOld, Qt::CaseInsensitive) && pos > 1) {
                __mapGlobal.insert(__cnpjOld, __tempMap);
                __tempMap.clear();
                __tempMap.insert(pos, evt);
                __cnpjOld = evt->getCnpj();
            } else {
                __tempMap.insert(pos, evt);
                __cnpjOld = evt->getCnpj();
            }
        }

        if(pos>0)
            __mapGlobal.insert(__cnpjAtu, __tempMap);
        __tempMap.clear();
    }
    if(!__mapGlobal.isEmpty())
        __tempMap = this->processarSaldosRegistros(__mapGlobal);
    return __tempMap;
}

QMap<int, Eventos *> BancoDeDados::atualizarSaldosRateioGuia(QMap<int, Eventos *> map, double rat)
{
    QMapIterator<int, Eventos*> __tempIterator(map);
    QMap<int, Eventos*> __mapSaldo;
    double __valNegTemp = 0.0;
    int __qtd_Div_Temp = 0;
    int pos = 0;

    while (__tempIterator.hasNext()) {
        __tempIterator.next();
        pos++;
        Eventos *eventos = new Eventos;
        eventos = __tempIterator.value();

        double __valorGuiaIns = eventos->getValorGps();
        double __valorDeducao = rat;
        double __valorEventos = 0.0;

        if((__valorGuiaIns < __valorDeducao) && __valorGuiaIns > 0.0) {
            __valNegTemp+=__valorDeducao-__valorGuiaIns;
            eventos->setValorGps(0.0);
            eventos->setValorDeducao(0.0);
        } else {
            double __diferenca = 0.0;
            if(__valorGuiaIns > 0.0) {
                __diferenca = __valorGuiaIns - __valorDeducao;
                __diferenca = __diferenca*(-1);
            }

            if(__diferenca > 0.0) {
                __valNegTemp+=__diferenca;
                eventos->setValorGps(0.0);
                eventos->setValorDeducao(0.0);
            } else {
                __qtd_Div_Temp++;
                if(__valorGuiaIns > 0.0)
                    __valorEventos = __valorGuiaIns - __valorDeducao;
                eventos->setValorGps(__valorEventos);
            }
        }
        __mapSaldo.insert(pos, eventos);
    }

    QString __saldoTemporario = QString::number(__valNegTemp, 'f', 2);

    __valNegTemp=__saldoTemporario.toDouble(nullptr);
    this->setDivisorSaldo(__qtd_Div_Temp);
    if(__valNegTemp > 0)
        this->setSaldoRateioNegativo(__valNegTemp);
    else
        this->setSaldoRateioNegativo(0.0);

    return __mapSaldo;
}

QMap<int, Eventos *> BancoDeDados::processarSaldosRegistros(QMap<QString, QMap<int, Eventos *> > __mapGlobal)
{
    QMapIterator<QString, QMap<int, Eventos*>>__itGlobal(__mapGlobal);
    QMap<int, Eventos*> __tempMap;
    QMap<int, Eventos*> __mapAgro;
    int __n_rand = 0;
    while (__itGlobal.hasNext()) {
        __itGlobal.next();
        this->setDivisorSaldo(0.0);
        this->setSaldoRateioNegativo(0.0);
        __tempMap.clear();
        __tempMap = __itGlobal.value();

        /*** Atualizando valores da GUIA para coletar o Saldo  Negativo para Rateio ***/
        QMapIterator<int, Eventos*> __mapIterator(__tempMap);
        __tempMap.clear();
        double valorNegativo = 0.0;
        int qtd_Div = 0;
        int pos = 0;
        while (__mapIterator.hasNext()) {
            __mapIterator.next();
            pos++;
            Eventos *eventos = new Eventos;
            eventos = __mapIterator.value();

            //Iniciando variaveis de fluxo
            double __valorDeducao = eventos->getValorDeducao();
            double __valorGuiaIns = eventos->getValorGps();
            double __valorEventos = eventos->getValorEvento();
            double __valorTerceir = eventos->getValorTerceiro();

            if(__valorGuiaIns < __valorDeducao) {
                //Tratamento quando o valor da deducao for maior que o INSS
                valorNegativo+=__valorDeducao - __valorGuiaIns;
                eventos->setValorGps(0.0);
                eventos->setValorDeducao(0.0);
            } else {
                //Tratamento quando o valor da deducao for menor que o INSS
                double __restoNegativo=__valorGuiaIns - __valorDeducao;
                __restoNegativo=__restoNegativo*-1;
                if(__restoNegativo > 0) {
                    valorNegativo+=__restoNegativo;
                    eventos->setValorDeducao(0.0);
                    eventos->setValorEvento(0.0);
                } else {
                    qtd_Div++;
                    __valorGuiaIns = __valorGuiaIns - __valorDeducao;
                    eventos->setValorGps(__valorGuiaIns);
                    eventos->setValorEvento(__valorEventos);
                    eventos->setValorTerceiro(__valorTerceir);
                }
            }
            __tempMap.insert(pos, eventos);
        }

        if( valorNegativo > 0 ) {
            this->setDivisorSaldo(qtd_Div);
            this->setSaldoRateioNegativo(valorNegativo);
            /*** Atualizando rateios atraves do loop para zerar o saldo negativo ***/
            double __tempSaldo = this->getSaldoRateioNegativo();
            int xSequencias = 0;
            while ( __tempSaldo > 0.01 ) {
                xSequencias++;
                double __n_rat = this->getSaldoRateioNegativo() / this->getDivisorSaldo();
                __tempMap = this->atualizarSaldosRateioGuia(__tempMap,__n_rat);
                __tempSaldo = this->getSaldoRateioNegativo();
            }
        }
        __mapIterator = __tempMap;
        __tempMap.clear();
        while (__mapIterator.hasNext()) {
            __mapIterator.next();
            __n_rand++;
            Eventos *eventos = new Eventos;
            eventos = __mapIterator.value();
            eventos->setValorEvento(eventos->getValorTerceiro() + eventos->getValorGps());
            __mapAgro.insert(__n_rand, eventos);
        }
    }
    __mapGlobal.clear();
    __tempMap.clear();
    return __mapAgro;
}

QMap<int, ObjetoRetencao *> BancoDeDados::getMetaRetencao(QDate __dataInicio, QDate __dataFinal)
{
    QMap<int, ObjetoRetencao *> __tempMap;
    QString comando = QString( "select a.numcad, "
                               "a.nomfun, "
                               "a.datafa, "
                               "a.datadm, "
                               "a.titcar, "
                               "a.nomloc, "
                               "a.caudem, "
                               "a.desdem, "
                               "a.numemp, "
                               "a.codfil, "
                               "a.nomfil, "
                               "a.nomrsl, "
                               "a.nroadm, "
                               "a.nrodem  "
                               "from view_meta_retencaoadm a  "
                               "where a.numemp > 1000 and a.datadm between ' %0 ' AND ' %1 ' "
                               "union "
                               "select d.numcad, "
                               "d.nomfun, "
                               "d.datafa, "
                               "d.datadm, "
                               "d.titcar, "
                               "d.nomloc, "
                               "d.caudem, "
                               "d.desdem, "
                               "d.numemp, "
                               "d.codfil, "
                               "d.nomfil, "
                               "d.nomrsl, "
                               "d.nroadm, "
                               "d.nrodem  "
                               "from view_meta_retencaodem d  "
                               "where d.numemp > 1000 and d.datafa between ' %0 ' AND ' %1 ' ")
            .arg(__dataInicio.toString(Qt::ISODate))
            .arg(__dataFinal.toString(Qt::ISODate));
    QSqlQuery consulta;
    consulta.setForwardOnly(true);
    consulta.prepare(comando);
    if(!consulta.exec()) {
        emit messagemRetorno(consulta.lastError().text());
        return __tempMap;
    } else {
        int pos = 0;
        while (consulta.next()) {
            pos++;
            ObjetoRetencao *retencao = new ObjetoRetencao();
            retencao->setCadastro( QVariant( consulta.value("numcad") ).toInt(nullptr) );
            retencao->setNome( QVariant( consulta.value("nomfun") ).toString() );
            retencao->setDemissao( QVariant( consulta.value("datafa") ).toDate() );
            retencao->setAdmissao( QVariant( consulta.value("datadm") ).toDate() );
            retencao->setCargo( QVariant( consulta.value("titcar") ).toString() );
            retencao->setSetor( QVariant( consulta.value("nomloc") ).toString() );
            retencao->setCodigoDaCausa( QVariant( consulta.value("caudem") ).toInt(nullptr) );
            retencao->setCausa( QVariant( consulta.value("desdem") ).toString() );
            retencao->setCodigoDaEmpresa( QVariant( consulta.value("numemp") ).toInt(nullptr) );
            retencao->setCodigoDaFilial( QVariant( consulta.value("codfil") ).toInt(nullptr) );
            retencao->setFilial( QVariant( consulta.value("nomfil") ).toString() );
            retencao->setRecrutador( QVariant( consulta.value("nomrsl") ).toString() );
            retencao->setAdmitidos( QVariant( consulta.value("nroadm") ).toInt(nullptr) );
            retencao->setDemitidos( QVariant( consulta.value("nrodem") ).toInt(nullptr) );
            __tempMap.insert(pos, retencao);
        }
    }
    return __tempMap;
}

QMap<int, CadastroColaborador *> BancoDeDados::getColaboradoresAtivos(QString __ID_Empresa, QString __ID_Filial, QDate __dataReferencia)
{
    QMap<int, CadastroColaborador*> __tempMap;
    QString __parametroGeral;

    if(__dataReferencia.year() < 2013) {
        __parametroGeral = QString(" < 1000 ");
    } else {
        if(__ID_Empresa.isEmpty()) {
            __parametroGeral = QString(" > 1000 ");
            if(!__ID_Filial.isEmpty())
                __parametroGeral.append(QString(" AND hfi.codfil IN ( ").append(__ID_Filial).append(" ) "));
        } else {
            __parametroGeral = QString(" IN ( ").append(__ID_Empresa).append(" ) ");
            if(!__ID_Filial.isEmpty())
                __parametroGeral.append(QString(" AND hfi.codfil IN ( ").append(__ID_Filial).append(" ) "));
        }
    }
    QString comando = QString("        SELECT "
                              "        fun.numemp, "
                              "        fil.razsoc, "
                              "        hfi.codfil, "
                              "        fil.nomfil, "
                              "        cid.nomcid, "
                              "        fil.numcgc, "
                              "        fun.numcad, "
                              "        fun.numcpf, "
                              "        fun.numpis, "
                              "        fun.nomfun, "
                              "        fun.datadm, "
                              "        fun.datnas, "
                              "        hvi.codvin, "
                              "        hlo.taborg, "
                              "        hlo.numloc, "
                              "        hie.codloc, "
                              "        orn.nomloc, "
                              "        hca.estcar, "
                              "        hca.codcar, "
                              "        car.titcar, "
                              "        hsa.tipsal, "
                              "        hsa.valsal, "
                              "        fun.deffis, "
                              "        sin.nomsin, "
                              "        sin.numcgc cgcsin "
                              " FROM   r034fun fun "
                              "        INNER JOIN r038hvi hvi "
                              "                ON hvi.numemp = fun.numemp "
                              "                   AND hvi.tipcol = fun.tipcol "
                              "                   AND hvi.numcad = fun.numcad "
                              "                   AND hvi.datalt = (SELECT Max(tb0.datalt) "
                              "                                     FROM   r038hvi tb0 "
                              "                                     WHERE  tb0.numemp = hvi.numemp "
                              "                                            AND tb0.tipcol = hvi.tipcol "
                              "                                            AND tb0.numcad = hvi.numcad "
                              "                                            AND tb0.datalt <= ' %0 ') "
                              "        INNER JOIN r038hfi hfi "
                              "                ON hfi.numemp = fun.numemp "
                              "                   AND hfi.tipcol = fun.tipcol "
                              "                   AND hfi.numcad = fun.numcad "
                              "                   AND hfi.datalt = (SELECT Max(tb1.datalt) "
                              "                                     FROM   r038hfi tb1 "
                              "                                     WHERE  tb1.numemp = hfi.numemp "
                              "                                            AND tb1.tipcol = hfi.tipcol "
                              "                                            AND tb1.numcad = hfi.numcad "
                              "                                            AND tb1.datalt <= ' %0 ') "
                              "        INNER JOIN r038hlo hlo "
                              "                ON hlo.numemp = fun.numemp "
                              "                   AND hlo.tipcol = fun.tipcol "
                              "                   AND hlo.numcad = fun.numcad "
                              "                   AND hlo.datalt = (SELECT Max(tb2.datalt) "
                              "                                     FROM   r038hlo tb2 "
                              "                                     WHERE  tb2.numemp = hlo.numemp "
                              "                                            AND tb2.tipcol = hlo.tipcol "
                              "                                            AND tb2.numcad = hlo.numcad "
                              "                                            AND tb2.datalt <= ' %0 ') "
                              "        INNER JOIN r038hca hca "
                              "                ON hca.numemp = fun.numemp "
                              "                   AND hca.tipcol = fun.tipcol "
                              "                   AND hca.numcad = fun.numcad "
                              "                   AND hca.datalt = (SELECT Max(tb3.datalt) "
                              "                                     FROM   r038hca tb3 "
                              "                                     WHERE  tb3.numemp = hca.numemp "
                              "                                            AND tb3.tipcol = hca.tipcol "
                              "                                            AND tb3.numcad = hca.numcad "
                              "                                            AND tb3.datalt <= ' %0 ') "
                              "        INNER JOIN r038hsa hsa "
                              "                ON hsa.numemp = fun.numemp "
                              "                   AND hsa.tipcol = fun.tipcol "
                              "                   AND hsa.numcad = fun.numcad "
                              "                   AND hsa.datalt = (SELECT Max(tb4.datalt) "
                              "                                     FROM   r038hsa tb4 "
                              "                                     WHERE  tb4.numemp = hsa.numemp "
                              "                                            AND tb4.tipcol = hsa.tipcol "
                              "                                            AND tb4.numcad = hsa.numcad "
                              "                                            AND tb4.datalt <= ' %0 ') "
                              "                   AND hsa.seqalt = (SELECT Max(tb5.seqalt) "
                              "                                     FROM   r038hsa tb5 "
                              "                                     WHERE  tb5.numemp = hsa.numemp "
                              "                                            AND tb5.tipcol = hsa.tipcol "
                              "                                            AND tb5.numcad = hsa.numcad "
                              "                                            AND tb5.datalt = hsa.datalt) "
                              "         LEFT JOIN r038hsi hsi "
                              "                ON hsi.numemp = fun.numemp "
                              "                   AND hsi.tipcol = fun.tipcol "
                              "                   AND hsi.numcad = fun.numcad "
                              "                   AND hsi.datalt = (SELECT Max(tb6.datalt) "
                              "                                     FROM   r038hsi tb6 "
                              "                                     WHERE  tb6.numemp = hsi.numemp "
                              "                                            AND tb6.tipcol = hsi.tipcol "
                              "                                            AND tb6.numcad = hsi.numcad "
                              "                                            AND tb6.datalt <= ' %0 ') "
                              "        INNER JOIN r022vin vin  "
                              "                ON vin.codvin = hvi.codvin "
                              "                   AND vin.calfol = 'S' "
                              "        INNER JOIN r030fil fil  "
                              "                ON fil.numemp = fun.numemp "
                              "                   AND fil.codfil = hfi.codfil "
                              "        INNER JOIN r074cid cid  "
                              "                ON cid.estcid = fil.codest "
                              "                   AND cid.codcid = fil.codcid "
                              "        INNER JOIN r016hie hie  "
                              "                ON hie.taborg = hlo.taborg "
                              "                   AND hie.numloc = hlo.numloc "
                              "                   AND hie.datini <= ' %0 ' "
                              "                   AND hie.datfim >= ' %0 ' "
                              "        INNER JOIN r016orn orn  "
                              "                ON orn.taborg = hlo.taborg "
                              "                   AND orn.numloc = hlo.numloc "
                              "        INNER JOIN r024car car  "
                              "                ON car.estcar = hca.estcar "
                              "                   AND car.codcar = hca.codcar "
                              "        INNER JOIN r014sin sin  "
                              "                ON sin.codsin = hsi.codsin "
                              " WHERE  fun.numemp %1 "
                              "        AND fun.datadm <= ' %0 ' "
                              "        AND NOT EXISTS (SELECT 1 "
                              "                        FROM   r038afa afa "
                              "                        WHERE  afa.numemp = fun.numemp "
                              "                               AND afa.tipcol = fun.tipcol "
                              "                               AND afa.numcad = fun.numcad "
                              "                               AND afa.datafa = (SELECT Max(tb0.datafa) "
                              "                                                 FROM   r038afa tb0, "
                              "                                                        r010sit t01 "
                              "                                                 WHERE  tb0.numemp = afa.numemp "
                              "                                                        AND tb0.tipcol = afa.tipcol "
                              "                                                        AND tb0.numcad = afa.numcad "
                              "                                                        AND tb0.sitafa = t01.codsit "
                              "                                                        AND t01.tipsit = 7 "
                              "                                                        AND tb0.datafa <= ' %0 ')) "
                              " ORDER  BY fun.numemp, hfi.codfil, fun.numcad ")
            .arg(__dataReferencia.toString(Qt::ISODate))
            .arg(__parametroGeral);
    QSqlQuery consulta;
    consulta.prepare(comando);
    if(!consulta.exec()) {
        emit messagemRetorno(consulta.lastError().text());
        return __tempMap;
    } else {
        int pos = 0;
        if(consulta.isActive()) {
            while (consulta.next()) {
                CadastroColaborador *colaborador = new CadastroColaborador();
                colaborador->setCodigoDaEmpresa( QVariant( consulta.value( 0 )).toString());
                colaborador->setEmpresa( QVariant( consulta.value( 1 )).toString());
                colaborador->setCodigoDaFilial( QVariant( consulta.value( 2 )).toString());
                colaborador->setFilial( QVariant( consulta.value( 3 )).toString());
                colaborador->setCidadeRegiao( QVariant( consulta.value( 4 )).toString());
                colaborador->setCNPJ( QVariant( consulta.value( 5 )).toString());
                colaborador->setMatricula( QVariant( consulta.value( 6 )).toString());
                colaborador->setCPF( QVariant( consulta.value( 7 )).toString());
                colaborador->setPIS( QVariant( consulta.value( 8 )).toString());
                colaborador->setNome( QVariant( consulta.value( 9 )).toString());
                colaborador->setDataDeAdmissao( QVariant( consulta.value( 10 )).toDate());
                colaborador->setDataDeNascimento( QVariant( consulta.value( 11 )).toDate());
                colaborador->setCodigoDeVinculo( QVariant( consulta.value( 12 )).toString());
                colaborador->setTabelaDeOrganograma( QVariant( consulta.value( 13 )).toString());
                colaborador->setNumeroDoLocal( QVariant( consulta.value( 14 )).toString());
                colaborador->setHierarquiaDeLocal( QVariant( consulta.value( 15 )).toString());
                colaborador->setSetor( QVariant( consulta.value( 16 )).toString());
                colaborador->setEstruturaDeCargos( QVariant( consulta.value( 17 )).toString());
                colaborador->setCodigoDoCargo( QVariant( consulta.value( 18 )).toString());
                colaborador->setCargo( QVariant( consulta.value( 19 )).toString());
                colaborador->setTipoDeSalario( QVariant( consulta.value( 20 )).toInt(nullptr));
                colaborador->setSalario( QVariant( consulta.value( 21 )).toDouble(nullptr));
                colaborador->setSindicatoNome( QVariant( consulta.value( 23 )).toString());
                colaborador->setSindicatoCNPJ( QVariant( consulta.value( 24 )).toString());
                __tempMap.insert(pos, colaborador);
                pos++;
            }
        }
    }
    return __tempMap;
}

QMap<int, ObjetoCrednossoRetorno *> BancoDeDados::getCrednossoRetorno(QDate __competencia)
{
    QDate __folhaMens(__competencia.year(), __competencia.month(), 1);
    QDate __iniPeriodo(__competencia.year(), __competencia.month()+1, 1);
    int ultimoDiaMes = 0;

    if( ((__competencia.month()+1)%2) == 0)
        ultimoDiaMes = 30;
    else
        ultimoDiaMes = 31;

    QDate __fimPeriodo(__competencia.year(), __competencia.month()+1, ultimoDiaMes);
    QString __FOLHA = QString(" AND cal.inicmp = ' %0 ' ").arg(__folhaMens.toString(Qt::ISODate));
    QString __FERIAS = QString(" AND fev.inifer BETWEEN ' %0 ' AND ' %1 ' ").arg(__iniPeriodo.toString(Qt::ISODate)).arg(__fimPeriodo.toString(Qt::ISODate));

    QMap<int, ObjetoCrednossoRetorno *> __tempMap;
    QString comando = QString(" WITH crednossofolha (numemp, numcpf, razsoc, codfil, nomfil, numcgc, numcad, "
                              "      nomfun, "
                              "      codeve, deseve, valeve) "
                              "      AS (SELECT ver.numemp, "
                              "                 fun.numcpf, "
                              "                 fil.razsoc, "
                              "                 hfi.codfil, "
                              "                 fil.nomfil, "
                              "                 fil.numcgc, "
                              "                 ver.numcad, "
                              "                 fun.nomfun, "
                              "                 ver.codeve, "
                              "                 evc.deseve, "
                              "                 ver.valeve "
                              "          FROM   r046ver ver "
                              "                 INNER JOIN r044cal cal "
                              "                         ON cal.numemp = ver.numemp "
                              "                            AND cal.codcal = ver.codcal "
                              "                AND cal.tipcal = 11 "
                              "                 INNER JOIN r008evc evc "
                              "                         ON evc.codtab = ver.tabeve "
                              "                            AND evc.codeve = ver.codeve "
                              "                 INNER JOIN r034fun fun "
                              "                         ON fun.numemp = ver.numemp "
                              "                            AND fun.tipcol = ver.tipcol "
                              "                            AND fun.numcad = ver.numcad "
                              "                 INNER JOIN r038hfi hfi "
                              "                         ON hfi.numemp = ver.numemp "
                              "                            AND hfi.tipcol = ver.tipcol "
                              "                            AND hfi.numcad = ver.numcad "
                              "                            AND hfi.datalt = (SELECT Max(tb0.datalt) "
                              "                                              FROM   r038hfi tb0 "
                              "                                              WHERE  tb0.numemp = hfi.numemp "
                              "                                                     AND tb0.tipcol = hfi.tipcol "
                              "                                                     AND tb0.numcad = hfi.numcad "
                              "                                                     AND tb0.numemp = tb0.empatu "
                              "                                                     AND tb0.numcad = tb0.cadatu "
                              "                           AND tb0.datalt <= cal.perref) "
                              "                 INNER JOIN r030fil fil "
                              "                         ON fil.numemp = ver.numemp "
                              "                            AND fil.codfil = hfi.codfil "
                              "          WHERE  ver.numemp > 1000 "
                              "                 AND ver.codeve = 1767"
                              "                 %0 "
                              "      ), "
                              "      crednossoferias (numemp, numcpf, razsoc, codfil, nomfil, numcgc, numcad, "
                              "      nomfun, codeve, deseve, valeve) "
                              "      AS (SELECT fev.numemp, "
                              "                 fun.numcpf, "
                              "                 fil.razsoc, "
                              "                 hfi.codfil, "
                              "                 fil.nomfil, "
                              "                 fil.numcgc, "
                              "                 fev.numcad, "
                              "                 fun.nomfun, "
                              "                 fev.codeve, "
                              "                 evc.deseve, "
                              "                 fev.valeve "
                              "          FROM   r040fev fev "
                              "                 INNER JOIN r008evc evc "
                              "                         ON evc.codtab = fev.tabeve "
                              "                            AND evc.codeve = fev.codeve "
                              "                 INNER JOIN r034fun fun "
                              "                         ON fun.numemp = fev.numemp "
                              "                            AND fun.tipcol = fev.tipcol "
                              "                            AND fun.numcad = fev.numcad "
                              "                 INNER JOIN r038hfi hfi "
                              "                         ON hfi.numemp = fev.numemp "
                              "                            AND hfi.tipcol = fev.tipcol "
                              "                            AND hfi.numcad = fev.numcad "
                              "                            AND hfi.datalt = (SELECT Max(tb0.datalt) "
                              "                                              FROM   r038hfi tb0 "
                              "                                              WHERE  tb0.numemp = hfi.numemp "
                              "                                                     AND tb0.tipcol = hfi.tipcol "
                              "                                                     AND tb0.numcad = hfi.numcad "
                              "                                                     AND tb0.numemp = tb0.empatu "
                              "                                                     AND tb0.numcad = tb0.cadatu) "
                              "                 INNER JOIN r030fil fil "
                              "                         ON fil.numemp = fev.numemp "
                              "                            AND fil.codfil = hfi.codfil "
                              "          WHERE  fev.numemp > 1000 "
                              "                 AND fev.codeve = 1209"
                              "                 %1 "
                              " ) "
                              " SELECT numemp, "
                              "        numcpf, "
                              "        razsoc, "
                              "        codfil, "
                              "        nomfil, "
                              "        numcgc, "
                              "        numcad, "
                              "        nomfun, "
                              "        codeve, "
                              "        deseve, "
                              "        valeve "
                              " FROM   crednossofolha "
                              " UNION "
                              " SELECT numemp, "
                              "        numcpf, "
                              "        razsoc, "
                              "        codfil, "
                              "        nomfil, "
                              "        numcgc, "
                              "        numcad, "
                              "        nomfun, "
                              "        codeve, "
                              "        deseve, "
                              "        valeve "
                              " FROM   crednossoferias "
                              " ORDER  BY numemp, "
                              "           codfil, "
                              "           numcad  ")
            .arg(__FOLHA).arg(__FERIAS);
    QSqlQuery consulta;
    consulta.setForwardOnly(true);
    consulta.prepare(comando);
    if(!consulta.exec()) {
        emit messagemRetorno(consulta.lastError().text());
        return __tempMap;
    } else {
        int pos = 0;
        while (consulta.next()) {
            pos++;
            ObjetoCrednossoRetorno *retorno = new ObjetoCrednossoRetorno;
            retorno->setCodigoDaEmpresa( QVariant( consulta.value("numemp") ).toInt(nullptr));
            retorno->setCPF( QVariant( consulta.value("numcpf") ).toInt(nullptr));
            retorno->setEmpresa( QVariant( consulta.value("razsoc") ).toString());
            retorno->setCodigoDaFilial( QVariant( consulta.value("codfil") ).toInt(nullptr));
            retorno->setFilial( QVariant( consulta.value("nomfil") ).toString());
            retorno->setCNPJ( QVariant( consulta.value("numcgc") ).toString());
            retorno->setMatricula( QVariant( consulta.value("numcad") ).toInt(nullptr));
            retorno->setNome( QVariant( consulta.value("nomfun") ).toString());
            retorno->setCodigoDoEvento( QVariant( consulta.value("codeve") ).toInt(nullptr));
            retorno->setEvento( QVariant( consulta.value("deseve") ).toString());
            retorno->setValor( QVariant( consulta.value("valeve") ).toDouble(nullptr));
            __tempMap.insert(pos, retorno);
        }
    }
    return __tempMap;
}

QMap<int, UpdateDataTableColumm *> BancoDeDados::getUpdateCadastroArquivo(int codigoEmpresa, int matricula, int linha, int coluna, bool ok)
{
    QMap<int, UpdateDataTableColumm *> __tempMap;
    QString comando = QString(" SELECT fun.nomfun, fun.codfil, fil.nomfil "
                              " FROM   r034fun fun "
                              "        INNER JOIN r030fil fil "
                              "                ON fil.numemp = fun.numemp "
                              "               AND fil.codfil = fun.codfil "
                              " WHERE  fun.numemp = %0 "
                              "    AND fun.numcad = %1 ").arg(codigoEmpresa).arg(matricula);
    QSqlQuery consulta;
    consulta.setForwardOnly(true);
    consulta.prepare(comando);
    if(!consulta.exec()) {
        emit messagemRetorno(consulta.lastError().text());
        return __tempMap;
    } else {
        while (consulta.next()) {
            UpdateDataTableColumm *cadastro = new UpdateDataTableColumm;
            cadastro->setLinha(linha);
            cadastro->setColuna(coluna);
            cadastro->setNome(consulta.value("nomfun").toString());
            cadastro->setCodigoFilial(consulta.value("codfil").toInt(nullptr));
            cadastro->setFilial(consulta.value("nomfil").toString());
            cadastro->setUltimoRegistro(ok);
            __tempMap.insert(0, cadastro);
        }
        return __tempMap;
    }
}

double BancoDeDados::getSaldoRateioNegativo() const
{
    return saldoRateioNegativo;
}

void BancoDeDados::setSaldoRateioNegativo(double value)
{
    saldoRateioNegativo = value;
}

int BancoDeDados::getDivisorSaldo() const
{
    return divisorSaldo;
}

void BancoDeDados::setDivisorSaldo(int value)
{
    divisorSaldo = value;
}

void BancoDeDados::statusProgressoAtual(int i, int t)
{
    emit statusProgresso(i,t);
}
