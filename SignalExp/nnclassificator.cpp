#include <QDebug>
#include <QtMath>
#include "res.h"
#include "sigmath.h"
#include "nnclassificator.h"
#include "dlgnnet.h"

NnClassificator::NnClassificator()
{
    _netType        = GostXX;//GostX;
    _reduceType     = ReduceByQMean;
    _respType       = BitCode;
    _decisionRule   = HMinRule;
    _connPercent    = 50.0;
}

NnClassificator::~NnClassificator(){
    for (int i=0; i<_nn.size(); ++i)
        if (_nn[i]) delete _nn[i];
}

SigClassificator::Descr NnClassificator::getStaticDescr(){
    Descr d = {
        1000,   //int     classes;    //максимальное число классов
        200000, //int     sizeX;      //число входов
        1,      //int     sizeY;      //число выходов
        true,   //bool    autoY;      //автоматическое назначение значений
        true,   //bool    foreignClassEnabled;    //разрешен класс "ВсеЧужие" без явного значения Y
        DiscreteValue,   //YFormat formatY;
        STR_RES("ГОСТ 52633"),
        STR_RES("nn")
    };
    return d;
}



SigClassificator::Descr NnClassificator::getDescr() const{
    return getStaticDescr();
}

QVector<VecF32*> NnClassificator::buildNormSamples(const QVector<VecF32*> &x, const VecF32 &ma, const VecF32 &sa){
    QVector<VecF32*> y(x.size());
    for (int i=0; i<x.size(); ++i){
        y[i] = new VecF32();
        *y[i] = sigmath::norm(*x[i],ma,sa);
    }
    return y;
}

void NnClassificator::freeNormSamples(QVector<VecF32*> &x){
    for (int i=0; i<x.size(); ++i) if (x[i]) delete x[i];
}


void NnClassificator::calcNormParams(const QVector<VecF32*> &all){
    //теперь можно вычислить мат. ожидание (поделив на все элементы)
    //а затем и дисперсию
    _ma = sigmath::mean(all);
    _sa = sigmath::stdev(all,_ma);
    float m=sigmath::mean(_ma);
    float s=sigmath::mean(_sa);
    qDebug()<<"ma mean="<<m<<" sa mean="<<s;
}

QVector<VecF32*> NnClassificator::buildAllSamples(QVector< QVector< VecF32* > > &x, bool hasAll){
    if (hasAll) return x.last();
    QVector<VecF32*> all;
    int count=0;    //общее число элементов выборки
    for (int k=0; k<x.size(); ++k) count+=x[k].size();
    all.reserve(count);
    for (int k=0; k<x.size(); ++k)
        for (int i=0; i<x[k].size(); ++i)
            all.append(x[k][i]);
    return all;
}

//Создание выходного кода
bool NnClassificator::buildYCode(int nclass, YCodeType codeType){
    if (nclass<=0 || nclass>256) return false;
    int ysize;
    switch (codeType){
    case NumericCode://числовой код 0,1,..3
        ysize=1;
        _y.resize(nclass);
        for (int i=0; i<nclass; ++i) { _y[i].resize(1); _y[i][0]=i; }
        _yunknown.resize(1);
        _yunknown[0]=-1;
        break;
    case BitCode:   //битовый код 00, 01, ..11
        if (nclass<=2)      ysize=1;
        else if (nclass<=4) ysize=2;
        else if (nclass<=8) ysize=3;
        else if (nclass<=16)ysize=4;
        else if (nclass<=32)ysize=5;
        else if (nclass<=64)ysize=6;
        else if (nclass<=128)ysize=7;
        else if (nclass<=256)ysize=8;
        else return false;  //не поддерживается
        _y.resize(nclass);
        for (int i=0; i<nclass; ++i) {
            _y[i].resize(ysize);
            for (int j=0; j<ysize; ++j) _y[i][j]=(i>>j)&1;
        }
        _yunknown.resize(ysize);
        for (int j=0; j<ysize; ++j) _yunknown[j]=-1;
        break;
    case PositionalCode://позиционный код 1000, 0100, ... 0001
        //длина кода будет совпадать с числом классов
        ysize=nclass;
        _y.resize(nclass);
        for (int i=0; i<nclass; ++i) {
            _y[i].resize(ysize);
            for (int j=0; j<ysize; ++j) _y[i][j]=0;
            _y[i][i]=1;
        }
        _yunknown.resize(ysize);
        for (int j=0; j<ysize; ++j) _yunknown[j]=0; //все 0
        break;
    default: //ничего не делать
        break;
    }
    return true;
}


//возвращает: 0, если успешно настроен
int NnClassificator::fit(QVector< QVector< VecF32* > > &x, QVector< VecF32 > &y){
    qDebug()<<"конфигурация:";
    qDebug()<<"связность="<<_connPercent<<"% порог качества="<<_reduceQ;
    qDebug()<<"тип сети="<<_netType<<" алгоритм прореживания="<<_reduceType<<" тип отклика="<<_respType;

    qDebug()<<"проверка параметров и генерация откликов";
    bool hasAll;
    _nclass = (y.size()) ? y.size() : x.size(); //число классов
    if (_nclass<=0 || _nclass>=100) { qDebug()<<"err -2"; return -2; }

    if (y.size()==0){
        hasAll=false;                           //указаны только классы Свой и выходной код не задан
        buildYCode(_nclass,_respType);          //построение эталонных кодов
    } else if (x.size()==y.size()){             //указаны только классы Свой
        hasAll=false;
        _y = y;
    } else if (x.size()==y.size()+1){   //указан класс Свой и Все Чужие
        hasAll=true;
        _y = y;
    } else {                            //указано неправильное число классов/выходных параметров
        qDebug()<<"err -1"<<"xsize="<<x.size()<<" ysize="<<y.size();
        return -1;
    }
    _xsize = x[0].size();
    _ysize = (_y.size())?_y[0].size():0;
    qDebug()<<"число классов="<<_nclass;

    //Найти мат. ожидание и дисперсию всех чужих
    //как сумму всех
    qDebug()<<"нормализация";
    QVector<VecF32*> all = buildAllSamples(x,hasAll);                   //Сформировать список параметров
    QVector< QVector<VecF32*> > own(x.size());
    calcNormParams(all);                                                 //Вычислить мат. ожидание и отклонение
    for (int i=0; i<x.size(); ++i) own[i]=buildNormSamples(x[i],_ma,_sa);//Нормализовать примеры Свой
    all = buildAllSamples(own,hasAll);
    qDebug()<<"обучение";
    //Обучить
    float q;
    switch (_reduceType){
    case NoReduce:      q=-1.0;     break; //не сокращать
    case ReduceByQMean: q=0.0;      break; //по среднему
    default:            q=_reduceQ; break; //по заданному качеству
    }

    _nn.resize(_nclass);
    for (int i=0; i<_nclass; ++i){
        switch (_netType){
        case GostX:     _nn[i] = new NNetGost();  break;
        case GostX2:    _nn[i] = new NNetGost2(); break;
        case GostX05:   _nn[i] = new NNetGost05(); break;
        case GostXX:    _nn[i] = new NNetGostGost(); break;
        default: qDebug()<<"тип сети не поддерживается"; return -1;
        }
    }

    for (int i=0; i<_nclass; ++i){
        _nn[i]->setConPart(_connPercent);//процент связей
        _nn[i]->setQThres(q);     //порог отбраковывания связей
        //Взять только 80% примеров свой (а тестирование вести на оставшихся примерах)
        int n80 = 4*own[i].size()/5;
        QVector<VecF32*> own80(n80);
        for (int j=0; j<n80; ++j) own80[j]=own[i][j];
        //Обучить
        if (0!=_nn[i]->fit(own80,all)){ qDebug()<<"ошибка обучения классу "<<i; return -3; }
    }

    //Тестировать на образах Свой и вычислить параметры Свой (а можно других классов)
    //мат.ожидание (6 и 20) и дисперсию (30 и 60)
    //эти значения можно использовать для принятия решения о близости к образу Свой
    //во время тестирования
    _my.resize(_nclass);
    _sy.resize(_nclass);
    for (int i=0; i<_nclass; ++i){
        double m=0.0;
        double s=0.0;
        for (int i2=0; i2<_nclass; ++i2){
            //Взять только тестовые примеры
            int n20 = own[i2].size() - 4*own[i2].size()/5;
            QVector<VecF32*> own20(n20);
            int n20start = own[i2].size()-n20;
            for (int j=0; j<n20; ++j) own20[j]=own[i2][n20start+j];

            for (int e=0; e<own20.size(); ++e){
                int ecur = _nn[i]->testH(*own20[e]);
                m += ecur;
                s += ecur*ecur;
            }
            m = m/n20;
            s = sqrt(fabs(s/n20-m*m));
            if (i==i2) { _my[i]=m; _sy[i]=s; }  //только при тестировании своего класса на своем классификаторе,
                                                //хотя можно было бы использовать и результаты тестирования других классов
                                                //(тогда целесообразнее было бы сделать еще один слой сети)
            qDebug()<<"test("<<i<<","<<i2<<") emean="<<m<<" estd="<<s<<" n="<<n20;
        }
    }
    //Освободить память из под вектора own
    for (int i=0; i<own.size(); ++i)
        freeNormSamples(own[i]);
    return 0;
}

//
int NnClassificator::evalRange(const VecI32 &h, const VecF32 &m, const VecF32 &s, VecF32 &p) const {
    p.resize(h.size()+1);//все классы Свой+ВсеЧужие
    //Оценить вероятность принадлежности к классу Свой_i
    float pall=1.0;
    int iclass=h.size()+1;
    for (int i=0; i<h.size(); ++i) {
        if ((h[i]>=(_my[i]-2.0*_sy[i])) && (h[i]<=(_my[i]+2.0*_sy[i]))) { iclass=i; p[i]=1.0; pall=0.0; }
        else { p[i]=0.0; }
    }
    return iclass;//номер макс. элемента
}

//h - мера Хэмминга текущего примера до класса Свой
//m - мат. ожидание класса Свой по мере Хэмминга
//s - отклонение класса Свой по мере Хэмминга
int NnClassificator::evalProb(const VecI32 &h, const VecF32 &m, const VecF32 &s, VecF32 &p) const {
    p.resize(h.size()+1);//все классы Свой+ВсеЧужие
    //Оценить вероятность принадлежности к классу Свой_i
    float pall=1.0;
    float psum=0.0;
    for (int i=0; i<h.size(); ++i) {
        p[i] = _erf.prob(h[i],_my[i],_sy[i]);
        pall = pall*(1.0-p[i]); //вероятность не принадлежности к классам Свой
        psum+=p[i];
    }
    psum+=pall;
    //Нормировать вероятности
    if (psum!=0.0)
    for (int i=0; i<p.size(); ++i)
        p[i]/=psum;
    return std::max_element(p.data(),p.data()+p.size())-p.data();//номер макс. элемента
}


//возвращает: номер класса
int NnClassificator::test(const VecF32 &x, VecF32 &y, VecF32 *p) const{
    VecF32 xx = sigmath::norm(x,_ma,_sa);
    //Для каждого из класса найти меру Хэмминга
    //можно ускорить, если нормирование выполнять на этом уровне, а не ниже
    //тогда нужно будет мат. ожидание чужих и своих передавать дальше
    VecI32 h(_nn.size());
    VecF32 ptmp;
    if (!p) p=&ptmp;
    for (int i=0; i<_nn.size(); ++i)
        h[i]=_nn[i]->testH(xx);
    int iclass=0;
    switch (_decisionRule){
    case HMinRule:      iclass = std::min_element(h.data(),h.data()+h.size())-h.data(); break;
    case HRangeRule:    iclass = evalRange(h,_my,_sy,*p); break;
    case HDensityRule:  iclass = evalProb(h,_my,_sy,*p); break;
    }
    if (iclass>h.size()){ //класс ВсеЧужие
        y = _yunknown;
        return -1;
    } else { //класс Свой
        if (_y.size()) y = _y[iclass];  //значение в пороге
        return iclass;
    }
}


void NnClassificator::config(ConfigPage page, QWidget *parent){
    DlgNNet dlg(parent);
    dlg.setFitParams((int)_netType,_connPercent,(int)_reduceType,_reduceQ,(int)_respType);
    dlg.setTestParams((int)_decisionRule,_my,_sy,_y);
    //установить значения
    if (dlg.exec()!=QDialog::Accepted) return;
    //считать значения
    if (page==FitPage){
        int nt,rd,rs;
        dlg.getFitParams(nt,_connPercent,rd,_reduceQ,rs);
        _netType    = (NetType)nt;
        _reduceType = (ReduceType)rd;
        _respType   = (YCodeType)rs;
    }
    //if (page==FitPage){ dlg.getFitParams(); }
    //if (page==TestPage){ dlg.getTestParams(); }
}











//построение структуры сети
//xcount - число входных параметров
//ycount - число выходов (число нейронов нейронов)
//ccount - число связей одного нейрона
//схема подключения - случайная по умолчанию
bool NNetGost::build(int xcount, int ycount, int ccount){
    qDebug()<<"NNetGost.build";
    if (xcount<=0 || ycount<=0 || ccount<=0) return false;
    _c.resize(ycount);
    _w.resize(ycount);
    srand(0);   //посев генератора
    for (int k=0; k<ycount; ++k){
        _c[k].resize(ccount);
        _w[k].resize(ccount);
        for (int i=0; i<ccount; ++i){
            _c[k][i] = xcount*rand()/RAND_MAX;   //случайным образом выбрать номера связей
            _w[k][i] = 0.0;
        }
    }
    return true;
}

void NNetGost::reduceByMask(int ineuron, const VecI32 &mask){
    VecI32 &c = _c[ineuron];
    VecF32 &w = _w[ineuron];
    int j=0;
    for (int i=0; i<c.size(); ++i){
        if (mask[c[i]]){//оставить только включенные в маску
            c[j]=c[i];
            w[j]=w[i];
            ++j;
        }
    }
    c.resize(j);
    w.resize(j);
}

bool NNetGost::buildByMask(int xcount, int ycount, int ccount, const VecI32 &mask){
    if (xcount<=0 || ycount<=0 || ccount<=0) return false;
    _c.resize(ycount);
    _w.resize(ycount);
    VecI32 con;
    int m=0;
    srand(0);   //посев генератора
    QDebug *dbg = new QDebug(qDebug());
    for (int k=0; k<ycount; ++k){
        con.clear();
        for (int i=0; i<ccount; ++i){
            int c=(xcount-1)*rand()/RAND_MAX; //случайным образом выбрать номера связей
            if (!mask[c])  continue;          //и отбросить, если они не в маске
            con.push_back(c);                 //примечание: таким образом, число связей будет переменным
        }
        _c[k] = con;
        _w[k].resize(con.size());
        m+=con.size();
    }
    delete dbg;
    qDebug()<<"среднее число связей = "<<m/_c.size();
    return true;
}

//обучение одному классу
//примечание: в функцию подаются уже нормированные примеры
//own - примеры образа Свой
//all - примеры образа ВсеЧужие
int NNetGost::fit( const QVector<VecF32*> &own, const QVector<VecF32*> &all){
    Q_UNUSED(all);
    qDebug()<<"обучение классу..";
    //Задать структуру нейронной сети по умолчанию
    int ycount = 256;               //число нейронов
    int xcount = own[0]->size();    //число входов

    VecF32 m = sigmath::mean(own);
    VecF32 s = sigmath::stdev(own,m);
    VecF32 q = sigmath::qual(m,s,20);
    float qm = sigmath::mean(q);
    qDebug()<<"m="<<sigmath::mean(m)<<" s="<<sigmath::mean(s)<<" q="<<qm;
    if (_qThres<0.0)        qm=-1.0;      //оставить все значения
    else if (_qThres>0.0)   qm = _qThres; //по заданному порогу
                                          //по среднему значению (уже вычислено)
    //Составить маску и убрать нейроны с качеством, ниже заданного
    VecI32 mask(xcount);
    for (int i=0; i<xcount; ++i) mask[i] = (q[i]>=qm)?1:0;
    if (!buildByMask(xcount,ycount,(int)(_conPart*xcount/100.0),mask)) { qDebug()<<"err build"; return -1; }

    //Установить знаки весов в соответствии с знаком выходного кода (>0 или <=0)
    for (int j=0; j<ycount; ++j){
        for (int i=0; i<_c[j].size(); ++i){
            if (-1.0*m[_c[j][i]]>0.0)  _w[j][i]=1.0;
            else                       _w[j][i]=-1.0;
        }
    }
    //Установить значения весов по качеству (пропорционально)
    for (int j=0; j<ycount; ++j){
        for (int i=0; i<_c[j].size(); ++i)
            _w[j][i]*=q[_c[j][i]];
    }

    //Подсчитать расстояние
    QVector<VecF32> o3(own.size());     //данные
    QDebug* dbg = new QDebug(qDebug());
    *dbg<<"hamm:\n";
    int hcount=0;
    for (int i=0; i<own.size(); ++i) {
        test(*own[i],o3[i]);
        int h=0; for (int e=0; e<o3[i].size(); ++e) if (o3[i][e]>0) ++h;
        if (h) hcount++;
        *dbg<<h<<' ';
    }
    *dbg<<"[ненулевых "<<hcount<<"]";
    delete dbg;

    return 0;
}

//подсчет расстояния Хэмминга до эталонного кода 00000
int NNetGost::testH(const VecF32 &x) const{
    int h=0;
    for (int i=0; i<_c.size(); ++i)
        h+= sim(i,x)>0;
    return h;
}


//преобразование одного примера
void NNetGost::test(const VecF32 &x, VecF32 &y) const{
    y.resize(_c.size());
    for (int i=0; i<_c.size(); ++i)
        y[i]=sim(i,x);
}

void NNetGost::test_linear(const VecF32 &x, VecF32 &y) const{
    y.resize(_c.size());
    for (int i=0; i<_c.size(); ++i)
        y[i]=sim_linear(i,x);
}

float NNetGost::sim_linear(int ineuron, const VecF32 &x) const{     //эмуляция одного нейрона
    float y=0.0;
    const VecI32 &c = _c[ineuron];
    const VecF32 &w = _w[ineuron];
    for (int i=0; i<c.size(); ++i) y+=x[c[i]]*w[i];
    if (y<-1.0) return -1.0;
    if (y>1.0) return 1.0;
    return y;
}


int NNetGost::sim(int ineuron, const VecF32 &x) const{     //эмуляция одного нейрона
    float y=0.0;
    const VecI32 &c = _c[ineuron];
    const VecF32 &w = _w[ineuron];
    for (int i=0; i<c.size(); ++i) y+=x[c[i]]*w[i];
    return (y>0.0)? 1:-1;
}

VecF32    NNetGost::sim(int ineuron, const QVector<VecF32*> &x) const{ //эмуляция одного нейрона на нескольких примерах
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=sim(ineuron,*x[i]);
    return y;
}

VecF32    NNetGost::sim(int ineuron, const QVector<VecF32> &x) const{ //эмуляция одного нейрона на нескольких примерах
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=sim(ineuron,x[i]);
    return y;
}





//----------------------------------------------------------------------------------------------------
//Квадратичная сеть
//((w-x)/s)^2
//----------------------------------------------------------------------------------------------------

int NNetGost2::fit( const QVector<VecF32*> &own, const QVector<VecF32*> &all){
    Q_UNUSED(all);
    qDebug()<<"обучение классу..";
    //Задать структуру нейронной сети по умолчанию
    int ycount = 256;               //число нейронов
    int xcount = own[0]->size();    //число входов

    _m = sigmath::mean(own);
    _s = sigmath::stdev(own,_m);
    VecF32 q = sigmath::qual(_m,_s,20);
    float qm = sigmath::mean(q);
    qDebug()<<"m="<<sigmath::mean(_m)<<" s="<<sigmath::mean(_s)<<" q="<<qm;
    if (_qThres<0.0)        qm=-1.0;      //оставить все значения
    else if (_qThres>0.0)   qm = _qThres; //по заданному порогу
                                          //по среднему значению (уже вычислено)
    //Составить маску и убрать нейроны с качеством, ниже заданного
    VecI32 mask(xcount);
    for (int i=0; i<xcount; ++i) mask[i] = (q[i]>=qm)?1:0;
    if (!buildByMask(xcount,ycount,(int)(_conPart*xcount/100.0),mask)) { qDebug()<<"err build"; return -1; }

    //Предотвратить деление на 0
    for (int i=0; i<_s.size(); ++i) {
        if (fabs(_s[i])<min_sigma) _s[i]=min_sigma;//очень маленькое ненулевое значение
    }

    //Настроить порог Свой/Чужой (лучше рассчитывать вероятности по распределению)
    QDebug *dbg = new QDebug(qDebug());
    *dbg<<"town:";

    QVector<VecF32> v(ycount);
    for (int i=0; i<ycount; ++i){
        v[i] = mssum(i,own);
        std::sort(v[i].begin(),v[i].end());//отсортировать по возрастанию откликов
    }


    //VecF32 m2 = sigmath::mean(v,0,-1);
    //VecF32 s2 = sigmath::stdev(v,m2,0,-1);
    _thres.resize(ycount);
    for (int i=0; i<ycount; ++i){
        //_thres[i]=m2[i]+2*s2[i];  //порог среднее+2 сигма
        _thres[i] = v[i][(int)(v[i].size()*0.9)]; //взять порог 90% прохождения примеров Свой
        *dbg<<"("<<_thres[i]<<")";
    }
    //подбирать порог, при котором
    //проходят все свои и не проходят все чужие невозможно,
    //т.к. распределения будут пересекаться и нужно эмпирически
    //подбирать удовлетворяющий требованиям порог
    delete dbg;
    return 0;
}

float NNetGost2::mssum(int ineuron, const VecF32 &x) const {   //подсчет выходного значения (без порога
    float y=0.0;
    const VecI32 &c = _c[ineuron];
    for (int i=0; i<c.size(); ++i) {
        float z =(x[c[i]]-_m[c[i]])/_s[c[i]];
        y+=z*z;
    }
    return y;
}

VecF32 NNetGost2::mssum(int ineuron, const QVector<VecF32*> &x) const {
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=mssum(ineuron,*x[i]);
    return y;
}

int NNetGost2::mssim(int ineuron, const VecF32 &x) const { //эмуляция с порогом
    if (mssum(ineuron,x)>_thres[ineuron])   return 1;   //0 - Свой; 1 - Чужой
    else                                    return 0;
}

void NNetGost2::test(const VecF32 &x, VecF32 &y) const{
    y.resize(_c.size());
    for (int i=0; i<_c.size(); ++i)
        y[i]=mssim(i,x);
}

int NNetGost2::testH(const VecF32 &x) const{
    int y=0;
    for (int i=0; i<_c.size(); ++i) y+=mssim(i,x);
    return y;
}




//----------------------------------------------------------------------------------------------------
//Корень квадратный сеть
//((w-x)/s)^0.5
//----------------------------------------------------------------------------------------------------
float NNetGost05::mssum(int ineuron, const VecF32 &x) const {   //подсчет выходного значения (без порога
    float y=0.0;
    const VecI32 &c = _c[ineuron];
    for (int i=0; i<c.size(); ++i) {
        float z =(x[c[i]]-_m[c[i]])/_s[c[i]];
        y+=::pow(::fabs(z),0.5);
    }
    return y;
}



//----------------------------------------------------------------------------------------------------
//Двухслойная сеть с разными линейными функциями
//----------------------------------------------------------------------------------------------------
int  NNetGostGost::fit( const QVector<VecF32*> &own, const QVector<VecF32*> &all){
    l1.fit(own,all);
    //так как все параметры нормированы по дисперсии, то
    //дисперсия распределения равна 1.
    //значит достаточно заменить нелинейную функцию линейной с насыщением в диапазоне [-1;+1]
    QVector<VecF32> o2(own.size());     //данные
    QVector<VecF32> a2(all.size());
    QVector<VecF32*> own2(own.size());  //ссылки
    QVector<VecF32*> all2(all.size());
    for (int i=0; i<own.size(); ++i) { l1.test_linear(*own[i],o2[i]); own2[i]=o2.data()+i;}
    for (int i=0; i<all.size(); ++i) { l1.test_linear(*all[i],a2[i]); all2[i]=a2.data()+i;}
    int rslt = l2.fit(own2,all2);

    //подсчитать выходное качество результата (используем ту же память, что и для второго слоя)
    QVector<VecF32> o3(own.size());     //данные
    //QVector<VecF32> a3(all.size());
    QVector<VecF32*> own3(own.size());  //ссылки
    //QVector<VecF32*> all3(all.size());
    //QDebug* dbg = new QDebug(qDebug());
    for (int i=0; i<own.size(); ++i) {
        l2.test(o2[i],o3[i]); own3[i]=o3.data()+i;
        //int h=0; for (int e=0; e<o3[i].size(); ++e) if (o3[i][e]>0) ++h;
        //*dbg<<h<<' ';
        //for (int e=0; e<o3[i].size(); ++e) *dbg<<o3[i][e]; *dbg<<'\n';
    }
    //delete dbg;
    //for (int i=0; i<all.size(); ++i) { l2.test(a2[i],a3[i]); all3[i]=a3.data()+i;}
    VecF32 m = sigmath::mean(own3);
    VecF32 s = sigmath::stdev(own3,m);
    VecF32 q = sigmath::qual(m,s,20);
    float qm = sigmath::mean(q);
    qDebug()<<"m3o="<<sigmath::mean(m)<<" s3o="<<sigmath::mean(s)<<" q3o="<<qm;
    //m = sigmath::mean(all3);
    //s = sigmath::stdev(all3,m);
    //q = sigmath::qual(m,s,20);
    //qm = sigmath::mean(q);
    //qDebug()<<"m3a="<<sigmath::mean(m)<<" s3a="<<sigmath::mean(s)<<" q3a="<<qm;

    return rslt;
}

void NNetGostGost::test(const VecF32 &x, VecF32 &y) const{
    l1.test_linear(x,*_y2);
    l2.test(*_y2,y);
}

int  NNetGostGost::testH(const VecF32 &x) const{
    l1.test_linear(x,*_y2);
    return l2.testH(*_y2);
}

void NNetGostGost::setConPart(float cp){
    l1.setConPart(cp);
    l2.setConPart(cp);
}

void NNetGostGost::setQThres(float q){
    l1.setQThres(q);
    l2.setQThres(q);
}




