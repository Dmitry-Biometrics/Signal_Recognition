#ifndef SIGCLASSIFICATOR_H
#define SIGCLASSIFICATOR_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include "sigdata.h"

class SigClassificator: public QObject {
    Q_OBJECT
signals:
    void onLog(const QString &text);
public:
    SigClassificator():_xsize(0),_ysize(0),_nclass(0){}

    const QString& name() const { return _name; }
    void setName(const QString& value){ _name = value; }

    virtual ~SigClassificator(){}
    virtual QVector< VecF32 > y() const { return _y; }      //эталонные отклики

    //virtual int batchSize() const { return _batch; }
    //virtual float testPerc() const { return _perc;  }
    //virtual int epoch() const     { return _epoch; }

    //virtual void setBatchSize(int value){ _batch = value; }    //установка размера выборки
    //virtual void setTestPerc(float percent){ _perc = percent; }//установка процента тестовых данных (от обучающей выборки)
    //virtual void setEpoch(int value) { _epoch = value; }       //установка числа эпох (если обучается нейронная сеть)

    enum ConfigPage {
        TestPage,
        FitPage
    };


    virtual void config(ConfigPage page, QWidget *parent=0){ Q_UNUSED(parent); } //настроить дополнительные параметры в отдельном диалоге

    enum YFormat {
        DiscreteValue,  // одиночное дискретное значение (-1/0/1/2/..)
        SingleValue,    // одиночное вещественное значение (0.1, 9.5)
        BinaryVector,   // бинарный вектор              (11111111)/(0000000)/(xxxxxxx)
        SparseVector,   // разреженный вектор           (1 единица, остальные нули)
        FloatVector,    // вектор вещественных значений (1.9,2.2,5.0)/(0.2,9.6,0.1)/..
    };

    struct Descr {
        int     classes;    //максимальное число классов
        int     sizeX;      //число входов
        int     sizeY;      //число выходов
        bool    autoY;      //автоматическое назначение значений
        bool    foreignClassEnabled;    //разрешен класс "ВсеЧужие" без явного значения Y
        YFormat formatY;
        QString name;       //имя метода
        QString en;         //короткое имя
    };

    virtual Descr getDescr() const = 0;

    //Обучение
    //  x[класс][пример][параметр]  входные данные
    //  y[класс][параметр]          ожидаемые отклики (может быть не задан)
    //возвращает результат обучения: 0 - успешно
    virtual int fit(QVector< QVector<VecF32*> > &x, QVector<VecF32> &y)=0;

    //Тестирование
    //  x[вх]  вх вектор
    //  y[вых] вектор откликов
    //  p[вых] вероятность принадлежности к классу {0,1,..,ни один из классов}
    //          размер вектора на 1 больше,чем getClassesCount()
    //возвращает: номер класса
    virtual int test(const VecF32 &x, VecF32 &y, VecF32 *p=0) const=0;


    virtual int fit(QVector<SigMarks* > &x, QVector<VecF32> &y);
    virtual int test(SigMarks& u) const;

    virtual int getXSize(){ return _xsize; }            //длина входного вектора
    virtual int getYSize(){ return _ysize; }            //длина выходного кода
    virtual int getClassesCount(){ return _nclass; }    //число классов

Q_SIGNALS:

    void onEpoch(const QStringList &report);

protected:
    QVector<VecF32 > _y;    //эталонные отклики (может быть не задан)
    QString _name;
    //int _batch;
    //int _perc;
    //int _epoch;
    int _xsize;
    int _ysize;
    int _nclass;
};


#endif // SIGNALCLASSIFICATOR_H
