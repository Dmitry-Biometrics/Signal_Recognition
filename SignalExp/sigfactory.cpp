#include "res.h"
#include "xi4classificator.h"
#include "statclassificator.h"
#include "mdclassificator.h"
#include "sigfactory.h"
#include "nnclassificator.h"

SigFactory::SigFactory()
{

}


SigFactory::~SigFactory()
{

}

SigClassificator::Descr SigFactory::getDescr(int id) const {
    //EuclidMode, NormEuclidMode, SVMMode, NBCCMode
    switch (id){
    case 0: return Xi2Classificator::getStaticDescr();
    case 1: return StatClassificator::getStaticDescr(EuclidMode);
    case 2: return StatClassificator::getStaticDescr(NormEuclidMode);
    case 3: return StatClassificator::getStaticDescr(SVMMode);
    case 4: return MdClassificator::getStaticDescr();
    case 5: return Xi4Classificator::getStaticDescr();
        //добавить нейросетевой классификатор
        //добавить нейросетевой квадратичный классификатор
    case 6: return NnClassificator::getStaticDescr();
    default: return SigClassificator::Descr();
    }
}

//Список поддерживаемых методов
QStringList SigFactory::types() const {
    return QStringList()
    <<STR_RES("Хи2")
    <<STR_RES("Евклид")
    <<STR_RES("Нормированный Евклид")
    <<STR_RES("Метод опорных векторов")
    <<STR_RES("Пропорционально отклонению")
    <<STR_RES("Хи4")
    <<STR_RES("Сеть ГОСТ x");
}

SigClassificator* SigFactory::create(int id) const{   //создание классификатора
    switch (id){
    case 0:  return new Xi2Classificator();
    case 1:  return new StatClassificator(EuclidMode);
    case 2:  return new StatClassificator(NormEuclidMode);
    case 3:  return new StatClassificator(SVMMode);
    case 4:  return new MdClassificator();
    case 5:  return new Xi4Classificator();

    case 6:  return new NnClassificator();
    default: return 0;
    }
}
