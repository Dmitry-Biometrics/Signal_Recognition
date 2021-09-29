#include "statclassificator.h"
#include "res.h"

StatClassificator::StatClassificator(ClassificatorMode mode):_mode(mode),_c(new SignalRecog()){
}

StatClassificator::~StatClassificator(){
    if (_c) delete _c;
}

int StatClassificator::fit(QVector< QVector< VecF32* > > &x, QVector< VecF32 > &y){
    _nclass = 2;
    _xsize = x[0].size();
    _ysize = 1;
    return _c->train(x[0],x[1],_mode);
}

int StatClassificator::test(const VecF32 &x, VecF32 &y, VecF32 *p) const{
    Q_UNUSED(p);
    int label;
    _c->predict(x,y,label);
    return label;
}

SigClassificator::Descr StatClassificator::getStaticDescr(ClassificatorMode mode){
    switch (mode){
    case EuclidMode:
    { Descr d = { 2, 1000000, 1, true, false, DiscreteValue, STR_RES("Евклид"), "euc" }; return d; }
    case NormEuclidMode:
    { Descr d = { 2, 1000000, 1, true, false, DiscreteValue, STR_RES("Нормированный Евклид"), "neuc"}; return d; }
    case SVMMode:
    { Descr d = { 2, 1000000, 1, true, false, DiscreteValue, STR_RES("Метод опорных векторов"), "svm"}; return d; }
    case NBCCMode:
    { Descr d = { 2, 1000000, 512, true, true, BinaryVector, STR_RES("Биометрия-код"), "nbcc"}; return d; }
    default: return Descr();
    }
}

SigClassificator::Descr StatClassificator::getDescr() const {
    return getStaticDescr(_mode);
}
