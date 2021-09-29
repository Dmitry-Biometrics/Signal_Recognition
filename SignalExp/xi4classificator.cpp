#include "res.h"
#include "xi4classificator.h"

Xi4Classificator::Xi4Classificator(){ _power = 4; }

SigClassificator::Descr Xi4Classificator::getStaticDescr(){
    Descr d = {
        1,      //int     classes;    //максимальное число классов
        1000000,//int     sizeX;      //число входов
        1,      //int     sizeY;      //число выходов
        true,   //bool    autoY;      //автоматическое назначение значений
        false,  //bool    foreignClassEnabled;    //разрешен класс "ВсеЧужие" без явного значения Y
        DiscreteValue,   //YFormat formatY;
        STR_RES("Хи 4"),
        STR_RES("xi4")
    };
    return d;
}

SigClassificator::Descr Xi4Classificator::getDescr() const {
    return getStaticDescr();
}
