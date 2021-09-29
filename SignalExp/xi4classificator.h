#ifndef XI4CLASSIFICATOR_H
#define XI4CLASSIFICATOR_H

#include "xi2classificator.h"

class Xi4Classificator:public Xi2Classificator {
public:
    Xi4Classificator();

    static Descr getStaticDescr();
    virtual Descr getDescr() const;
};

#endif // XI4CLASSIFICATOR_H
