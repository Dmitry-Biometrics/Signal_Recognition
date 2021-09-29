#ifndef STATCLASSIFICATOR_H
#define STATCLASSIFICATOR_H


#include "SignalRecog.h"
#include "sigclassificator.h"

class StatClassificator:public SigClassificator {
public:
    StatClassificator(ClassificatorMode mode);
    virtual ~StatClassificator();

    virtual int fit(QVector< QVector< VecF32* > > &x, QVector< VecF32 > &y);
    virtual int test(const VecF32 &x, VecF32 &y, VecF32 *p=0) const;

    static Descr getStaticDescr(ClassificatorMode mode);
    virtual Descr getDescr() const;
protected:
    ClassificatorMode _mode;
    SignalRecog *_c;
};

#endif // STATCLASSIFICATOR_H
