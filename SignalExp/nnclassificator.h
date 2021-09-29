#ifndef NNCLASSIFICATOR_H
#define NNCLASSIFICATOR_H

#include "sigclassificator.h"
#include "sigmath.h"

////Режимы обучения нейронной сети
//enum NNetGostMode {
//    GostXMode,      //настройка x по качеству
//    GostX2Mode,     //настройка x^2 по качеству
//    GostX05Mode,    //настройка x^0.5 по качеству
//    GostXXMode      //настройка многослойной сети
//};

class NNetLayer {
public:
    virtual ~NNetLayer(){}
    virtual int  fit( const QVector<VecF32*> &own, const QVector<VecF32*> &all)=0;
    virtual void test(const VecF32 &x, VecF32 &y) const=0;  //преобразование одного примера
    virtual int  testH(const VecF32 &x) const=0;            //мера Хэмминга до кода Свой
    virtual void setConPart(float cp)=0;                    //процент используемых одним нейронов связей
    virtual void setQThres(float q)=0;                      //порог по качеству для прореживания связей
};


class NNetGost:public NNetLayer {
public:
    NNetGost(){}
    virtual ~NNetGost(){}

    //обучение одному классу
    //own - примеры образа Свой
    //all - примеры образа ВсеЧужие
    //
    //mx - мат.ожидания {own,all}
    //sx - отклонения {own,all}
    virtual int fit( const QVector<VecF32*> &own, const QVector<VecF32*> &all);
    //преобразование одного примера
    virtual void test(const VecF32 &x, VecF32 &y) const;
    //Подсчитывает расстояние до кода 00000 по мере Хэмминга
    virtual int testH(const VecF32 &x) const;


    virtual void setConPart(float cp){ _conPart = cp; }  //процент используемых одним нейронов связей
    virtual void setQThres(float q){ _qThres = q; }      //порог, при формировании маски (значение 0.0 означает исп. маску по среднему)
                                                        //(значение меньше 0 позволяет пропустить в маску все значения)
    virtual void test_linear(const VecF32 &x, VecF32 &y) const;
protected:
    QVector<VecI32> _c;     //номера связей
    QVector<VecF32> _w;     //значения весовых коэффициентов

    float _qThres;
    float _conPart;

    int    sim(int ineuron, const VecF32 &x) const;     //эмуляция одного нейрона
    VecF32 sim(int ineuron, const QVector<VecF32*> &x) const; //эмуляция одного нейрона на нескольких примерах
    VecF32 sim(int ineuron, const QVector<VecF32> &x) const;

    float sim_linear(int ineuron, const VecF32 &x) const;

    //построение структуры сети
    //ncount - число нейронов
    //ccount - число связей одного нейрона
    //схема подключения - случайная по умолчанию
    bool build(int xcount, int ycount, int ccount);
    //построение карты сети по маске
    bool buildByMask(int xcount, int ycount, int ccount, const VecI32 &mask);
    //удаление связей по маске (если mask[i]=0, то номер i исключается)
    void reduceByMask(int ineuron, const VecI32 &mask);
};


class NNetGost2:public NNetGost {
public:
    virtual int fit( const QVector<VecF32*> &own, const QVector<VecF32*> &all);
    virtual void test(const VecF32 &x, VecF32 &y) const;
    virtual int testH(const VecF32 &x) const;
protected:
    static const float min_sigma = 0.000001;


    VecF32 _m;//ожидания Свой
    VecF32 _s;//отклонения Свой
    VecF32 _thres;//порог принятия решения "Свой"/"Чужой" по каждому выходу

    virtual float  mssum(int ineuron, const VecF32 &x) const;   //подсчет выходного значения (без порога)
    VecF32 mssum(int ineuron, const QVector<VecF32*> &x) const;
    int mssim(int ineuron, const VecF32 &x) const;
};

class NNetGost05:public NNetGost2 {
    virtual float  mssum(int ineuron, const VecF32 &x) const;   //подсчет выходного значения (без порога)
};

//двухслойная сеть
class NNetGostGost:public NNetLayer {
public:
    NNetGostGost(){ _y2=new VecF32(); }
    virtual ~NNetGostGost(){ delete _y2; }
    virtual int fit( const QVector<VecF32*> &own, const QVector<VecF32*> &all);
    virtual void test(const VecF32 &x, VecF32 &y) const;
    virtual int testH(const VecF32 &x) const;
    virtual void setConPart(float cp);
    virtual void setQThres(float q);
protected:
    static const float min_sigma = 0.000001;

    NNetGost l1;//первый слой
    NNetGost l2;//второй слой
    VecF32* _y2;//временный буфер для хранения отклика 1го слоя
};


class NnClassificator : public SigClassificator
{
public:
    NnClassificator();
    ~NnClassificator();

    //настройка классификатора (если число классов y меньше на 1 x, то последний класс - Все чужие)
    virtual int fit(QVector< QVector< VecF32* > > &x, QVector< VecF32 > &y);
    virtual int test(const VecF32 &x, VecF32 &y, VecF32 *p=0) const;

    static Descr getStaticDescr();
    virtual Descr getDescr() const;

    virtual void config(ConfigPage page, QWidget *parent=0);

    enum NetType {
        GostX,
        GostX2,
        GostX05,
        GostXX
    };

    enum ReduceType {
        NoReduce,
        ReduceByQMean,
        ReducebyQ
    };

    enum YCodeType {    //тип выходного кода (если не задан)
        UserCode,       //заданный пользователем
        NumericCode,    //номера {-1,0,1,2,3}
        BitCode,        //битовый код {00,01,10,11, nil}
        PositionalCode  //позиционный код {0001, 0010, 0100, 1000, nil}
    };

    enum YDecisionRule {  //правило принятия решения
        HMinRule,         //ближайший по мере Хэмминга (всегда выбирается)
        HRangeRule,       //ближайший в окрестностях   (m+-2s)
                          //значение вероятностей не нормируются

        HDensityRule      //по функции плотности вероятностей для известных значений (mean,std) каждого класса
                          //значение вероятностей нормируется
    };

protected:
    //построение эталонного кода
    //nclass   - число классов
    //codeType - тип кода
    virtual bool buildYCode(int nclass, YCodeType codeType);
    virtual QVector<VecF32*> buildAllSamples(QVector< QVector< VecF32* > > &x, bool hasAll);
    virtual void calcNormParams(const QVector<VecF32*> &all);
    //функция нормировки примеров
    //примечание: память под нормированные вектора выделяется заново! ее нужно освобождать delete
    virtual QVector<VecF32*> buildNormSamples(const QVector<VecF32*> &x, const VecF32 &ma, const VecF32 &sa);
    virtual void freeNormSamples(QVector<VecF32*> &x);

    //Определение вероятностей по классам Свой_i и ВсеЧужие
    //p - результирующий вектор вероятностей
    //возвращает: номер наиболее вероятного класса (по p)
    int evalProb(const VecI32 &h, const VecF32 &m, const VecF32 &s, VecF32 &p) const;

    //Определение вероятностей на основе принадлежности к диапазону
    //[m-2s;m+2s]
    int evalRange(const VecI32 &h, const VecF32 &m, const VecF32 &s, VecF32 &p) const;


protected:
    sigmath::Erf _erf;              //класс оценки вероятности принадлежности к классу
    sigmath::NormDistribution _nd;  //класс для вычисления нормального распределения

    NetType         _netType;
    ReduceType      _reduceType;
    YDecisionRule   _decisionRule;
    YCodeType       _respType;
    double          _connPercent;
    double          _reduceQ;

    VecF32 _yunknown;   //значение для неизвестного класса

    VecF32 _ma; //среднее по каждому параметру ВсеЧужие
    VecF32 _sa; //отклонение по каждому параметру ВсеЧужие

    //для каждого выходного значения
    VecF32 _my; //среднее (по мере хэмминга)
    VecF32 _sy; //отклонение (по мере хэмминга)
    //в правиле сравнения используются значения m=a*my+b; s=c*sy+b;
    //
    //Для вычисления вероятностей принадлежностей классам при использовании
    //нескольких классификаторов следует применять формулы:
    //p0 и p1 - вероятности, вычисленные для классификаторов 0 и 1 с учетом норм. распределения
    //px=(1-p0)*(1-p1); P=p0+p1+px
    //тогда результирующие вероятности {P0=p0/P, P1=p1/P, Px=px/P}

    QVector<NNetLayer*> _nn; //по одной нейронной сети для каждого класса
                            //каждый из которых обучается на примерах, исключающих указанный класс
                            //  0/1         - 2 шт
                            //  01/10/00/11 - 4 шт
                            //эталонный код для обучения 000...000
                            //процент использования связей одним нейроном 50%
    //в обучающую выборку ВсеЧужие включаются либо только отдельно подготовленные примеры, либо также все примеры из все других классов
};


#endif // NNCLASSIFICATOR_H
