#include "sigclassificator.h"

class Xi2Classificator:public SigClassificator {
public:
    Xi2Classificator();

    virtual int fit(QVector< QVector< VecF32* > > &x, QVector< VecF32 > &y);
    virtual int test(const VecF32 &x, VecF32 &y, VecF32 *p=0) const;

    static Descr getStaticDescr();
    virtual Descr getDescr() const;

    virtual void config(ConfigPage page, QWidget *parent=0);

protected:
    //масштабирование
    VecF32 scale(const VecF32 &sum, float minY, float  maxY) const;
    QVector<VecF32> scale(const QVector< VecF32* > &sum, float minY, float  maxY) const;
    void rescale(VecF32 &x, VecF32 &y, int nfrom, int nto) const;

    VecF32 mean(const QVector< VecF32 > &data) const;                         // среднее
    VecF32 stdev(const QVector< VecF32 > &data, const VecF32 &mean) const;    // отклонение

    virtual float xi2(const VecF32 &mean, const VecF32 &disp, const VecF32 &x) const;// хи квадрат
    //virtual float xi2threshold(const QVector< VecF32 > &data) const;  //порог
    virtual QPair<double,double> xi2range(const VecF32 &mean, const VecF32 &disp, const QVector< VecF32 > &data) const;  //порог

protected:
    //сохраняемые параметры обучения
    QPair<double,double> _xi2;   //порог
    VecF32 _mean;  //среднее
    VecF32 _disp;  //дисперсия
    float _power;
};
