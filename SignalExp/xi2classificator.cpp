#ifndef SIGNAL_CLASSIFICATION
#define SIGNAL_CLASSIFICATION

#include <QVector>
#include <QtMath>
#include "msg.h"
#include "res.h"
#include "xi2classificator.h"
#include "dlgxi2.h"

Xi2Classificator::Xi2Classificator(){ _power = 2.0; }


SigClassificator::Descr Xi2Classificator::getStaticDescr(){
    Descr d = {
        1,      //int     classes;    //максимальное число классов
        1000000,//int     sizeX;      //число входов
        1,      //int     sizeY;      //число выходов
        true,   //bool    autoY;      //автоматическое назначение значений
        false,  //bool    foreignClassEnabled;    //разрешен класс "ВсеЧужие" без явного значения Y
        DiscreteValue,   //YFormat formatY;
        STR_RES("Хи квадрат"),
        STR_RES("xi2")
    };
    return d;
}



SigClassificator::Descr Xi2Classificator::getDescr() const{
    return getStaticDescr();
}



VecF32 Xi2Classificator::scale(const VecF32 &x, float minY, float  maxY) const {
    if (!x.size()) return VecF32();
    VecF32 y; y.resize(x.size());
    float minX = x[0];
    float maxX = x[0];
    for (int i = 1; i<x.size(); i++)
        if (x[i]>maxX)      maxX = x[i];
        else if (x[i]<minX) minX = x[i];
    float dX = maxX-minX;
    float dY = maxY-minY;
    for (int i = 0; i<x.size(); i++)
        y[i] = (x[i]-minX)*dY/dX + minY;
    return y;
}

QVector<VecF32> Xi2Classificator::scale(const QVector< VecF32* > &x, float minY, float maxY) const {
    QVector<VecF32> y; y.reserve(x.size());
    for (int i=0; i<x.size(); ++i)
        y.push_back(scale(*x[i],minY,maxY));
    return y;
 }

VecF32 Xi2Classificator::mean(const QVector< VecF32 > &data) const {
    if (!data.size()) return VecF32();
    int nsamp = data.size();
    int n = data[0].size();
    VecF32 a(n,0.0);
    for (int i=0; i<n; ++i) {
        for (int j = 0;  j<nsamp; j++) a[i] += data[j][i];
        a[i]/=nsamp;
    }
    return a;
}


VecF32 Xi2Classificator::stdev(const QVector< VecF32 > &data, const VecF32 &mean) const {
    if (!data.size()) return VecF32();
    int nsamp = data.size();
    int n = data[0].size();
    VecF32 d(n);
    float inv = 1.0/_power;
    for (int i=0; i<n; ++i) {
        double dd=0.0;
        for (int j=0; j<nsamp; ++j) dd += pow(data[j][i]-mean[i],_power);
        d[i]= pow(dd/nsamp,inv);
    }
    return d;
}


float Xi2Classificator::xi2(const VecF32 &mean, const VecF32 &disp, const VecF32 &x) const {
    float y = 0.0f;
    for (int i=0; i<x.size(); ++i)
        y += pow((x[i]-mean[i])/disp[i],_power);
    //qDebug()<<"----------";
    //qDebug()<<"x: ";    for (int i=0; i<10; ++i) qDebug()<<x[i];
    //qDebug()<<"a: ";    for (int i=0; i<10; ++i) qDebug()<<mean[i];
    //qDebug()<<"d: ";    for (int i=0; i<10; ++i) qDebug()<<disp[i];
    //qDebug()<<"xi2: ";  for (int i=0; i<10; ++i) qDebug()<<pow((x[i]-mean[i])/disp[i],2);
    return y;
}



//Получить порог для значения хи-квадрат по обучающей выборке
//Для получения наиулучшего результата необходимо выбрать как минимум 20 образов, желательно 100
//float Xi2Classificator::xi2threshold(const QVector< VecF32 > &data) const {
//    int nsamp = data.size();
//    VecF32 a = mean(data);
//    VecF32 d = stdev(data,a);

//    float threshold=-1000.0;
//    for (int j=0; j<nsamp; ++j){
//        float x = xi2(a,d,data[j]);
//        if (x > threshold) threshold = x; //максимальный среди всех примеров
//    }
//    return threshold;
//}

QPair<double,double> Xi2Classificator::xi2range(const VecF32 &mean, const VecF32 &disp, const QVector< VecF32 > &data) const {
    QPair<double,double> r(1000000.0,-1000000.0);
    int nsamp = data.size();
    for (int j=0; j<nsamp; ++j){
        double x = xi2(mean,disp,data[j]);
        if (x<r.first) r.first = x;
        if (x>r.second) r.second = x;
    }
    return r;
}


void Xi2Classificator::rescale(VecF32 &x, VecF32 &y, int nfrom, int nto) const {
    VecF32 destx(nto);
    VecF32 desty(nto);
    float scale = nfrom/(float)nto;    // коэффициент масштабирования
    float rel = 0.0f;
    for(int i = 1; i < nto-1; i++){
        rel += scale;
        //Вычисление целочисленных индексов, близких к rel
        int l = (int)rel;       // меньший индекс
        int r = (int)rel + 1;   // больший индекс
        // Вычисление масштабированных координат (линейная интерполяция)
        if( r < (int)nfrom){
            destx[i] = ((rel - l)*x[r] + (r - rel)*x[l]);
            desty[i] = ((rel - l)*y[r] + (r - rel)*y[l]);
        } else {
            destx[i] = ((rel - l)*x[nfrom-1] + (r - rel)*destx[i-1]);
            desty[i] = ((rel - l)*y[nfrom-1] + (r - rel)*desty[i-1]);
        }
    }
    // Первые и последние координаты остаются без изменений
    destx[0] = x[0];
    desty[0] = y[0];
    destx[nto-1] = x[nfrom-1];
    desty[nto-1] = y[nfrom-1];
    x = destx;
    y = desty;
}


int Xi2Classificator::fit(QVector<QVector< VecF32* > > &x, QVector<VecF32> &y){
    Q_UNUSED(y);
    if (x.count()==0) return -1;
    _nclass = x.size();
    _xsize = x[0].size();
    _ysize = 1;

    QVector< VecF32 > data = scale(x[0],-1.0,1.0);
    _mean = mean(data);
    _disp = stdev(data,_mean);
    _xi2  = xi2range(_mean,_disp,data);  //вычислить порог xi2
    //qDebug()<<"fit xi2="<<_xi2.first<<";"<<_xi2.second;
    //qDebug()<<"mean:"<<_mean[0]<<" "<<_mean[1]<<" "<<_mean[2];
    //qDebug()<<"std:"<<_disp[0]<<" "<<_disp[1]<<" "<<_disp[2];
    //qDebug()<<"xi2:"<<_xi2;
    //qDebug()<<"fit";
    return 0;
}


int Xi2Classificator::test(const VecF32 &x, VecF32 &y, VecF32 *p) const {
    VecF32 data = scale(x,-1.0,1.0);
    y.resize(1);
    y[0] = xi2(_mean,_disp,data);
    //qDebug()<<"test y="<<y[0];
    return (y[0]<_xi2.first || y[0]>_xi2.second);
}





void Xi2Classificator::config(ConfigPage page,QWidget *parent){
    if (page!=TestPage) return;
    DlgXi2 dlg(_xi2,parent);
    if (dlg.exec()!=QDialog::Accepted) return;
    _xi2 = dlg.value();
}

#endif // SIGNAL_CLASSIFICATION


