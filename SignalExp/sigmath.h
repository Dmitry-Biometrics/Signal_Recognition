#ifndef SIGMATH
#define SIGMATH

#include "sigdata.h"

namespace sigmath {
    //приведение вектора к заданной длине
    VecF32 interp(const VecF32 &x, int nto);
    QVector<VecF32> interp(const QVector<VecF32> &x, int nto);
    QVector<VecF32> interp(const QVector<VecF32*> &x, int nto);

    //перевод вектора в частотную область
    VecF32 get_furie(const VecF32 &x);
    QVector<VecF32> get_furie(const QVector<VecF32> &x);
    QVector<VecF32> get_furie(const QVector<VecF32*> &x);

    //нормирование к диапазону [min_y,max_y]
    VecF32 norm(const VecF32 &x, float min_y, float max_y);
    QVector<VecF32> norm(const QVector<VecF32> &x, float min_y, float max_y);
    QVector<VecF32> norm(const QVector<VecF32*> &x, float min_y, float max_y);
    //нормирование y=(x-m)/s, где s!=0
    VecF32 norm(const VecF32 &x, const VecF32 &m, const VecF32 &s);
    QVector<VecF32> norm(const QVector<VecF32> &x, const VecF32 &m, const VecF32 &s);
    QVector<VecF32> norm(const QVector<VecF32*> &x, const VecF32 &m, const VecF32 &s);

    //мат. ожидание по элементам
    float  mean(const VecF32& x);
    VecF32 mean(const QVector<VecF32> &x, int start, int count);
    VecF32 mean(const QVector<VecF32*> &x, int start, int count);
    VecF32 mean(const QVector<VecF32*> &x);

    //отклонение по элементам
    VecF32 stdev(const QVector< VecF32 > &x, const VecF32 &mean, int start, int count);
    VecF32 stdev(const QVector< VecF32*> &x, const VecF32 &mean, int start, int count);
    VecF32 stdev(const QVector<VecF32*> &x, const VecF32 &mean);

    //качество y=mean/std
    VecF32 qual(const VecF32 &mean, const VecF32 &std, float maxq=1000.0f);

    VecF32 unite(const QVector<VecF32*> &x);
    VecF32 convI32ToF32(const VecI32 &x);


    //накопление сумм по каждому столбцу
    VecF32 sum(const QVector<VecF32> &x);     //значений
    VecF32 sum2(const QVector<VecF32> &x);    //квадрата значений
    VecF32 sum(const QVector<VecF32*> &x);    //значений
    VecF32 sum2(const QVector<VecF32*> &x);   //квадрата значений

    void   set_add(VecF32 &a, const VecF32 &b);  //поэлементное сложение a+=b

    float  sum(const VecF32 &x);
    float  sum2(const VecF32 &x);

    VecF32 div(const VecF32 &x, float k);
    VecF32 div(const VecF32 &x, const VecF32 &z);
    VecF32 mul(const VecF32 &x, float k);
    VecF32 mul(const VecF32 &x, const VecF32 &z);
    VecF32 sqrt(const VecF32 &x);

    float  max(const VecF32 &x);
    float  min(const VecF32 &x);


    float energy(const QVector<VecF32*> &x);
    float energy(const QVector<VecF32>  &x);


    enum BinOp {
        Add,
        Sub,
        Mul,
        Div,
        Pow,
    };

    enum UnOp {
        Neg,
        Abs,
        Int,
        Frac,
        Round,
        Exp,
        Pow2,
        Log2,
        LogE,
        Log10,
        Hev,
        Sin,
        Cos,
        Tan,
        ASin,
        ACos,
        ATan
    };

    QVector<VecF32> binOp(BinOp op, const QVector<VecF32> &x, const QVector<VecF32> &y);

    QVector<VecF32> unOp(UnOp op, const QVector<VecF32*> &x);
    QVector<VecF32> unOp(UnOp op, const QVector<VecF32> &x);

    //static QVector<VecF32> spline();
    //static QVector<VecF32> filter();

    //Округление с заданной точностью
    // x -  [in] округляемое значение
    // digits - [in] количество цифр после нуля
    float roundUp( float x, int digits );

    // Вычисление значения плотности вероятности нормального распределения для заданного X
    // x    - [in] значение для которого вычисляется плотность вероятности
    // aver - [in] математическое ожидание распределения
    // disp - [in] стандартное отклонение
    // возвращает: вычисленное значение плотности вероятности со знаком, относительно 0
    //double pnormx(double x, double aver, double disp);

    //Вычисление значения плотности вероятности в точки x
    //для нормального распределения с параметрами (0.0, 1.0)
    double dnorm(double x);

    // Вычисление вероятности (со знаком) распределения относительно начала координат
    // aver - [in] математическое ожидание распределения
    // disp - [in] стандартное отклонение
    // a    - [in] левая граница интервала
    // b    - [in] правая граница интервала
    // возвращает: значение вероятности
    float pnormxR32(float aver, float disp, float a, float b);
    void pnormxR32(const float *aver, const float *disp, unsigned nelms, float a, float b, float *prob);


    //Вычисление логистической функции ошибок (последовательность A007680)
    //https://oeis.org/A007680
    //примечание: для нормального распределения
    //  Если набор случайных величин подчиняется нормальному распределению со стандартным отклонением s
    //  то вероятность, что величина отклонится от среднего не более чем на a, т.е. a=|x-m|
    //  erf(a/(s*sqrt(2)))
    double erf(double x);
    double erf_prec(double x, int prec=1000);   //повышенной точности (prec задает число суммируемых членов)
    //Вероятность того, что величина x отклонится от среднего m равна
    double erf_pnorm(double x, double m, double s);

    //Нормальное распределение
    //примечание: точность цикла xnorm, pnorm на старших значениях 2 знака, на младших 4 знака.
    //            точность pnorm до 4 знаков
    class NormDistribution {
    public:
      NormDistribution();
      ~NormDistribution();
      //Значение плотности вероятности в точке x
      double dnorm(double x);
      double dnorm(double x, double aver, double disp);
      //Функция накопленной вероятности в интервале (-oo,x]
      //для закона с параметрами (aver=0.0,disp=1.0)
      double cnorm(double x);
      //Функция вероятности в интервале (a,b]
      //для закона с параметрами (aver=0.0,disp=1.0)
      double cnorm(double a, double b);

      //Функция вероятности в интервале (-oo,x] или [a,b] для закона с заданным средним и отклонением
      double pnorm(double x, double aver, double disp);
      double pnorm(double a, double b, double aver, double disp);

      //Нахождение (-oo;x) для заданной вероятности p
      //примечание: для нахождения b при известно p интервала (a,b),
      //нужно сложить p интервала (a,b и p интервала (-oo;a)
      double xnorm(double p);
      double xnorm(double p, double aver, double disp);

      //rnorm() Генерация случайных чисел в заданном распределении
    protected:
      enum { tbl_size = 16*1024 };
      double* tbl;
      double ue;//остаток в -oo
      double oe;//остаток в +oo
    };


    class Erf{
    public:
        Erf();
        ~Erf();
        double eval(double x) const;                      //оценка
        double prob(double x, double m, double s) const;  //вероятность принадлежности множеству с норм. распределением (m;s)
    protected:
        enum{ tbl_size = 16*1024 }; //для диапазона [-3;3] вне диапазона возвращает [-1;1]
        double * tbl;//таблица предварительно вычисленных значений
    };


}

#endif // SIGMATH

