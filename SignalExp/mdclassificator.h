#ifndef MDCLASSIFICATOR_H
#define MDCLASSIFICATOR_H

#include "sigclassificator.h"




//Дискретизирующий и выбирающий по мере Хэмминга классификатор
//mean(A)     |       mean(B)
// граница пропорционально отклонению
class MdClassificator: public SigClassificator {
public:
    MdClassificator();
    virtual ~MdClassificator();

    virtual int fit(QVector< QVector< VecF32* > > &x, QVector< VecF32 > &y);
    virtual int test(const VecF32 &x, VecF32 &y, VecF32 *p) const;

    static Descr getStaticDescr();
    virtual Descr getDescr() const;

    virtual void config(QWidget *parent=0);

private:
    VecF32          _m;
    QVector<bool>   _less;
    int             _thres;
};

#endif // MDCLASSIFICATOR_H
