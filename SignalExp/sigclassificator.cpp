#include "sigclassificator.h"


int SigClassificator::fit(QVector<SigMarks* > &x, QVector< VecF32> &y){
    QVector< QVector<VecF32*> > xx;
    xx.reserve(x.size());
    foreach(SigMarks* m, x) xx.push_back(m->x());
    return fit(xx,y);
}

int SigClassificator::test(SigMarks& u) const {
    for (int i=0; i<u.count(); ++i)
        u[i]._lb[1] = test(u[i].x(),u[i]._y);
    return u.count();
}
