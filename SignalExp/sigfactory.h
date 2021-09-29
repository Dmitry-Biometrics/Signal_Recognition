#ifndef SIGFACTORY_H
#define SIGFACTORY_H

#include <QSize>
#include "sigclassificator.h"

//Фабрика методов обработки данных
class SigFactory{
public:
    SigFactory();
    ~SigFactory();

    QStringList types() const;    //список поддерживаемых методов
    SigClassificator* create(int id) const;   //создание классификатора
    SigClassificator::Descr getDescr(int id) const;//описание классификатора

};

#endif // SIGFACTORY_H
