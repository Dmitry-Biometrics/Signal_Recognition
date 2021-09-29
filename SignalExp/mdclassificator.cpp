#include <QVector>
#include <QtMath>
#include "msg.h"
#include "res.h"
#include "sigmath.h"
#include "mdclassificator.h"

MdClassificator::MdClassificator()
{

}

MdClassificator::~MdClassificator()
{

}

SigClassificator::Descr MdClassificator::getStaticDescr(){
    Descr d = {
        2,      //int     classes;    //максимальное число классов
        1000000,//int     sizeX;      //число входов
        1,      //int     sizeY;      //число выходов (один - мера Хемминга)
        true,   //bool    autoY;      //автоматическое назначение значений
        false,  //bool    foreignClassEnabled;    //разрешен класс "ВсеЧужие" без явного значения Y
        DiscreteValue,   //YFormat formatY;
        STR_RES("Пропорционально отклонению"),
        STR_RES("md")
    };
    return d;
}



SigClassificator::Descr MdClassificator::getDescr() const{
    return getStaticDescr();
}


int MdClassificator::fit(QVector< QVector< VecF32* > > &x, QVector< VecF32 > &y){
    Q_UNUSED(y);
    Q_ASSERT(x.size()==2 && x[0].size()>0 && x[1].size()>0);
    QVector<VecF32*> &in0 = x[0];
    QVector<VecF32*> &in1 = x[1];
    //Вычислить ожидание и отклонение
    VecF32 m0 = sigmath::mean(in0,0,-1);
    VecF32 m1 = sigmath::mean(in1,0,-1);
    VecF32 d0 = sigmath::stdev(in0,m0,0,-1);
    VecF32 d1 = sigmath::stdev(in1,m1,0,-1);

    //Найти точку между m0 и m1 обратно пропорционально отклонению
    //Принимать решение в зависимости от того в какой стороне m0 и m1
    _m.resize(m0.size());
    _less.resize(m0.size());
    _thres=m0.size()/2;
    for (int i=0; i<_m.size(); ++i){
        //if ((d0[i]+d1[i]) == 0) _m[i] = (m0[i]+m1[i])/2;
        //else                    _m[i] = (m0[i]*d1[i]+m1[i]*d0[i])/(d0[i]+d1[i]);
        _m[i] = (m0[i]+m1[i])/2;
        _less[i] = (m0[i]<=m1[i]);
        qDebug()<<"m=("<<m0[i]<<" "<<_m[i]<<" "<<m1[i]<<") d=("<<d0[i]<<" "<<d1[i]<<")";
    }
    return 0;
}

int MdClassificator::test(const VecF32 &x, VecF32 &y, VecF32 *p) const {
    Q_UNUSED(p);
    Q_ASSERT(_m.size()==x.size());
    y.resize(1);
    int val=0;
    for (int i=0; i<_m.size();++i)
        if (x[i]<_m[i] && _less[i]){}
        else val++;
    y[0] = val;
    if (val<_thres) return 0;
    else            return 1;
}


void MdClassificator::config(QWidget *parent){
    Q_UNUSED(parent);
}



















































