#include <QtMath>
#include <QDebug>
#include <assert.h>
#include "sigmath.h"

#include <complex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
#include "Alglib\fasttransforms.h"

#define M_PI 3.14159265358979323846
using namespace std;
typedef complex<double> w_type;


namespace sigmath {


VecF32 interp(const VecF32 &x, int nto){
    int nfrom = x.size();
    VecF32 destx(nto);
    float scale = nfrom/(float)nto;    // коэффициент масштабирования
    float rel = 0.0f;
    for(int i = 1; i < nto-1; i++){
        rel += scale;
        //Вычисление целочисленных индексов, близких к rel
        int l = (int)rel;       // меньший индекс
        int r = (int)rel + 1;   // больший индекс
        // Вычисление масштабированных координат (линейная интерполяция)
        if( r < (int)nfrom){
            destx[i] = ((rel - l)*x[r] + (r - rel)*x[l]);
        } else {
            destx[i] = ((rel - l)*x[nfrom-1] + (r - rel)*destx[i-1]);
        }
    }
    // Первые и последние координаты остаются без изменений
    destx[0] = x[0];
    destx[nto-1] = x[nfrom-1];
    return destx;
}

QVector<VecF32> interp(const QVector<VecF32> &x, int nto){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32 &i, x) y.push_back(interp(i,nto));
    return y;
}

QVector<VecF32> interp(const QVector<VecF32*> &x, int nto){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32* i, x) y.push_back(interp(*i,nto));
    return y;
}


VecF32 get_furie(const VecF32 &x){

    VecF32 destx(x.size()/2);


    using namespace alglib;
    real_1d_array x2;   x2.setlength(x.size());
    for (int i = 0; i <x.size(); i++)	 x2[i]=x[i];
    complex_1d_array f;
    fftr1d(x2, f);
    for (int i=0; i<x.size(); i++) f[i] = pow(abscomplex(f[i]),2);
    real_1d_array x3;
    fftr1dinv(f, x3);

    for (int i=0; i<x.size()/2; i++){ destx[i] = abscomplex(f[i]); }



    return destx;
}

QVector<VecF32> get_furie(const QVector<VecF32> &x){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32 &i, x) y.push_back(get_furie(i));
    return y;
}

QVector<VecF32> get_furie(const QVector<VecF32*> &x){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32* i, x) y.push_back(get_furie(*i));
    return y;
}

VecF32 norm(const VecF32 &x, float min_y, float  max_y){
    int n = x.size();
    VecF32 y(n);
    float min = 0;
    float max = 0;
    for (int i = 0; i<n; i++) if (x[i]>max) max = x[i]; min = max;
    for (int i = 0; i<n; i++) if (x[i]<min) min = x[i];
    for (int i = 0; i<n; i++) y[i] = (((x[i]-min)*(max_y-min_y))/(max-min)) + min_y;
    return y;
}


QVector<VecF32> norm(const QVector<VecF32> &x, float min_y, float max_y){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32 &i, x) y.push_back(norm(i,min_y,max_y));
    return y;
}


QVector<VecF32> norm(const QVector<VecF32*> &x, float min_y, float max_y){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32* i, x) y.push_back(norm(*i,min_y,max_y));
    return y;
}

VecF32 norm(const VecF32 &x, const VecF32 &m, const VecF32 &s){
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=(x[i]-m[i])/s[i];
    return y;
}

QVector<VecF32> norm(const QVector<VecF32> &x, const VecF32 &m, const VecF32 &s){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32& i, x) y.push_back(norm(i,m,s));
    return y;
}

QVector<VecF32> norm(const QVector<VecF32*> &x, const VecF32 &m, const VecF32 &s){
    QVector<VecF32> y; y.reserve(x.size());
    foreach(const VecF32* i, x) y.push_back(norm(*i,m,s));
    return y;
}


void set_add(VecF32 &a, const VecF32 &b){  //поэлементное сложение a+=b
    assert(a.size()==b.size());
    for (int i=0; i<a.size(); ++i)
        a[i]+=b[i];
}

VecF32 sum(const QVector<VecF32*> &x){    //значений
    //примечание: считаем, что все векторы заданы и их длина одинаковая
    int n=x.size();    assert(n);
    int e=x[0]->size();assert(e);
    VecF32 y(e,0.0);
    for (int j=0; j<n; ++j){
        const VecF32 &xj =*x[j];
        for (int i=0; i<e; ++i) y[i]+=xj[i];
    }
    return y;
}

VecF32 sum2(const QVector<VecF32*> &x){   //квадрата значений
    int n=x.size();    assert(n);
    int e=x[0]->size();assert(e);
    VecF32 y(e,0.0);
    for (int j=0; j<n; ++j){
        const VecF32 &xj =*x[j];
        for (int i=0; i<e; ++i) y[i]+=xj[i]*xj[i];
    }
    return y;
}

VecF32 sum(const QVector<VecF32> &x){    //значений
    //примечание: считаем, что все векторы заданы и их длина одинаковая
    int n=x.size();     assert(n);
    int e=x[0].size();  assert(e);
    VecF32 y(e,0.0);
    for (int j=0; j<n; ++j){
        const VecF32 &xj =x[j];
        for (int i=0; i<e; ++i) y[i]+=xj[i];
    }
    return y;
}

VecF32 sum2(const QVector<VecF32> &x){   //квадрата значений
    int n=x.size();     assert(n);
    int e=x[0].size();  assert(e);
    VecF32 y(e,0.0);
    for (int j=0; j<n; ++j){
        const VecF32 &xj =x[j];
        for (int i=0; i<e; ++i) y[i]+=xj[i]*xj[i];
    }
    return y;
}

float sum(const VecF32 &x){
    float y=0.0;
    for (int i=0; i<x.size(); ++i) y+=x[i];
    return y;
}

float sum2(const VecF32 &x){
    float y=0.0;
    for (int i=0; i<x.size(); ++i) y+=x[i]*x[i];
    return y;
}


VecF32 div(const VecF32 &x, float k){
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=x[i]/k;
    return y;
}

VecF32 div(const VecF32 &x, const VecF32 &z){
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=x[i]/z[i];
    return y;
}

VecF32 mul(const VecF32 &x, float k){
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=x[i]*k;
    return y;
}

VecF32 mul(const VecF32 &x, const VecF32 &z){
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=x[i]*z[i];
    return y;
}

VecF32 sqrt(const VecF32 &x){
    VecF32 y(x.size());
    for (int i=0; i<x.size(); ++i) y[i]=::sqrt(x[i]);
    return y;
}


float  max(const VecF32 &x){
    if (!x.size()) return 0;
    return *std::max_element(x.data(),x.data()+x.size());
}

float  min(const VecF32 &x){
    if (!x.size()) return 0;
    return *std::min_element(x.data(),x.data()+x.size());
}


float energy(const QVector<VecF32> &x){
    int n = x.count();
    if (!n) return 0;
    int  c = 0;
    float e = 0.0;
    for (int i=0; i<n; ++i) {
        c+=x[i].size();            //число элементов
        e+=sigmath::sum2(x[i]);    //сумма квадратов по всем отсчетам
    }
    return ::sqrt(e/c);
}

float energy(const QVector<VecF32*> &x){
    int n = x.count();
    if (!n) return 0;
    int  c = 0;
    float e = 0.0;
    for (int i=0; i<n; ++i) {
        c+=x[i]->size();            //число элементов
        e+=sigmath::sum2(*x[i]);    //сумма квадратов по всем отсчетам
    }
    return ::sqrt(e/c);
}







float  mean(const VecF32& x){
    return sum(x)/x.size();
}

VecF32 mean(const QVector<VecF32> &x, int start, int count){
    if (count<0) count=x.size()-start;
    int n=x[start].size();
    VecF32 a(n,0.0);
    for (int i=0; i<n; ++i) {
        for (int j = start;  j<start+count; j++) a[i] += x[j][i];
        a[i]/=count;
    }
    return a;
}


VecF32 stdev(const QVector< VecF32 > &x, const VecF32 &mean, int start, int count){
    if (count<0) count=x.size()-start;
    int n = x[start].size();
    VecF32 d(n,0.0);
    for (int i=0; i<n; ++i) {
        for (int j=start; j<(start+count); ++j) { float v = (x[j][i]-mean[i]); d[i] += v*v; }
        d[i]= ::sqrt(d[i]/count);
    }
    return d;
}


VecF32 mean(const QVector<VecF32*> &x){
    VecF32 y = sum(x); int n=x.size();
    for (int i=0; i<y.size(); ++i) y[i]/=n;
    return y;
}


VecF32 mean(const QVector<VecF32*> &x, int start, int count){
    if (count<0) count=x.size()-start;
    int n=x[start]->size();
    VecF32 a(n,0.0);
    for (int i=0; i<n; ++i) {
        for (int j = start;  j<start+count; j++) a[i] += (*x[j])[i];
        a[i]/=count;
    }
    return a;
}

VecF32 stdev(const QVector<VecF32*> &x, const VecF32 &mean){
    VecF32 s = sum2(x);
    int n=x.size();
    int e=s.size();
    for (int i=0; i<e; ++i)
        s[i]=::sqrt(::fabs(s[i]/n-mean[i]*mean[i]));
    return s;
}


VecF32 stdev(const QVector< VecF32* > &x, const VecF32 &mean, int start, int count){
    if (count<0) count=x.size()-start;
    int n = x[start]->size();
    VecF32 d(n,0.0);
    for (int i=0; i<n; ++i) {
        for (int j=start; j<(start+count); ++j) { float v = ((*x[j])[i]-mean[i]); d[i] += v*v; }
        d[i]= ::sqrt(d[i]/count);
    }
    return d;
}


VecF32 qual(const VecF32 &mean, const VecF32 &std, float maxq){
    VecF32 q; q.resize(mean.size());
    if (std.size()==q.size())
    for (int i=0; i<q.size(); ++i){
        if (std[i]==0.0f) q[i]=maxq;
        else              q[i]=fabs(mean[i])/std[i];
    }
    return q;
}



QVector<VecF32> binOp(BinOp op, const QVector<VecF32> &x, const QVector<VecF32> &y){
    int n = (x.size()>y.size())?y.size():x.size();
    QVector<VecF32> z;
    z.reserve(n);
    for (int j=0; j<n; ++j){
        const VecF32 &a = x[j];
        const VecF32 &b = y[j];
        int k = (a.size()>b.size())?b.size():a.size();
        VecF32 v(k);
        switch (op){
        case Add:   for (int i=0; i<k; ++i) v[i] = a[i]+b[i]; break;
        case Sub:   for (int i=0; i<k; ++i) v[i] = a[i]-b[i]; break;
        case Mul:   for (int i=0; i<k; ++i) v[i] = a[i]*b[i]; break;
        case Div:   for (int i=0; i<k; ++i) v[i] = a[i]/b[i]; break;
        case Pow:   for (int i=0; i<k; ++i) v[i] = pow(a[i],b[i]); break;
        default: break;
        }
        z.push_back(v);
    }
    return z;
}

QVector<VecF32> unOp(UnOp op, const QVector<VecF32*> &x){
    int n = x.size();
    QVector<VecF32> z;
    z.reserve(n);
    for (int j=0; j<n; ++j){
        const VecF32 &a = *x[j];
        int k = a.size();
        VecF32 v(k);
        switch (op){
        case Neg:   for (int i=0; i<k; ++i) v[i] = -a[i]; break;
        case Abs:   for (int i=0; i<k; ++i) v[i] = (a[i]>0.0)?a[i]:-a[i]; break;

        case Int:   for (int i=0; i<k; ++i) v[i] = (int)a[i]; break;
        case Frac:  for (int i=0; i<k; ++i) v[i] = a[i]-trunc(a[i]); break;
        case Round: for (int i=0; i<k; ++i) v[i] = round(a[i]); break;

        case Exp:   for (int i=0; i<k; ++i) v[i] = exp(a[i]); break;
        case Pow2:  for (int i=0; i<k; ++i) v[i] = a[i]*a[i]; break;
        case Log2:  for (int i=0; i<k; ++i) v[i] = log2(a[i]); break;
        case LogE:  for (int i=0; i<k; ++i) v[i] = log(a[i]); break;
        case Log10: for (int i=0; i<k; ++i) v[i] = log10(a[i]); break;

        case Hev:   for (int i=0; i<k; ++i) v[i] = (a[i]>=0.0)?1.0:-1.0; break;

        case Sin:   for (int i=0; i<k; ++i) v[i] = sin(a[i]); break;
        case Cos:   for (int i=0; i<k; ++i) v[i] = cos(a[i]); break;
        case Tan:   for (int i=0; i<k; ++i) v[i] = tan(a[i]); break;
        case ASin:  for (int i=0; i<k; ++i) v[i] = asin(a[i]); break;
        case ACos:  for (int i=0; i<k; ++i) v[i] = acos(a[i]); break;
        case ATan:  for (int i=0; i<k; ++i) v[i] = atan(a[i]); break;
        default: break;
        }
        z.push_back(v);
    }
    return z;
}

QVector<VecF32> unOp(UnOp op, const QVector<VecF32> &x){
    int n = x.size();
    QVector<VecF32> z;
    z.reserve(n);
    for (int j=0; j<n; ++j){
        const VecF32 &a = x[j];
        int k = a.size();
        VecF32 v(k);
        switch (op){
        case Neg:   for (int i=0; i<k; ++i) v[i] = -a[i]; break;
        case Abs:   for (int i=0; i<k; ++i) v[i] = (a[i]>0.0)?a[i]:-a[i]; break;

        case Int:   for (int i=0; i<k; ++i) v[i] = (int)a[i]; break;
        case Frac:  for (int i=0; i<k; ++i) v[i] = a[i]-trunc(a[i]); break;
        case Round: for (int i=0; i<k; ++i) v[i] = round(a[i]); break;

        case Exp:   for (int i=0; i<k; ++i) v[i] = exp(a[i]); break;
        case Pow2:  for (int i=0; i<k; ++i) v[i] = a[i]*a[i]; break;
        case Log2:  for (int i=0; i<k; ++i) v[i] = log2(a[i]); break;
        case LogE:  for (int i=0; i<k; ++i) v[i] = log(a[i]); break;
        case Log10: for (int i=0; i<k; ++i) v[i] = log10(a[i]); break;

        case Hev:   for (int i=0; i<k; ++i) v[i] = (a[i]>=0.0)?1.0:-1.0; break;

        case Sin:   for (int i=0; i<k; ++i) v[i] = sin(a[i]); break;
        case Cos:   for (int i=0; i<k; ++i) v[i] = cos(a[i]); break;
        case Tan:   for (int i=0; i<k; ++i) v[i] = tan(a[i]); break;
        case ASin:  for (int i=0; i<k; ++i) v[i] = asin(a[i]); break;
        case ACos:  for (int i=0; i<k; ++i) v[i] = acos(a[i]); break;
        case ATan:  for (int i=0; i<k; ++i) v[i] = atan(a[i]); break;
        default: break;
        }
        z.push_back(v);
    }
    return z;
}




VecF32 unite(const QVector<VecF32*> &x){
    int count=0;
    VecF32 v;
    for (int i=0; i<x.size(); ++i) count+=x[i]->size();
    v.reserve(count);
    for (int i=0; i<x.size(); ++i)
        for (int j=0; j<x[i]->size(); ++j)
            v.push_back((*x[i])[j]);
    return v;
}

VecF32 convI32ToF32(const VecI32 &x){
    VecF32 v; v.resize(x.size());
    for (int i=0; i<x.size(); ++i) v[i] = (float)x[i];
    return v;
}



//Вычисление логистической функции ошибок (последовательность A007680)
//https://oeis.org/A007680
//примечание: для нормального распределения
//  Если набор случайных величин подчиняется нормальному распределению со стандартным отклонением s
//  то вероятность, что величина отклонится от среднего не более чем на a, т.е. a=|x-m|
//  erf(a/(s*sqrt(2)))
double erf(double x){
    //31 члена достаточно, чтобы обеспечить точность в диапазоне [-3,3] 3 сигма
    double xn=x, x2=xn*xn, y=x;
    double f=1.0;
    for (int i=1; i<31; ++i){ //31 элемент работает до [-3,3]
        f*=i; xn*=x2;
        double k = 1/(f*(2*i+1));
        if (i&1) y-=xn*k; else y+=xn*k;
    }
    y*=M_2_SQRTPI;
    if (y>1.0) return 1.0;
    if (y<-1.0) return -1.0;
    return y;
}

double erf_prec(double x, int n){
    //при числе элементов 100 приближает корректно до 6
    //при числе элементов 1000 приближает корректно до 6
    double y=x;
    double f=1.0;
    for (int i=1; i<n; ++i){
        f*=i;
        y+=pow(-1,i)*pow(x,2*i+1)/(f*(2*i+1));
    }
    return M_2_SQRTPI*y;
}

//Вероятность того, что величина x отклонится от среднего m равна
double erf_pnorm(double x, double m, double s){
    return erf(M_SQRT1_2*fabs(x-m)/s);
}


// Вычисление значения плотности вероятности нормального распределения для заданного X
// x    - [in] значение для которого вычисляется плотность вероятности
// aver - [in] математическое ожидание распределения
// disp - [in] стандартное отклонение
// возвращает: вычисленное значение плотности вероятности со знаком, относительно 0
double dnorm(double x, double aver, double disp){
  if (disp == 0.0) return 0.0;
  double deg = -1.0*pow((x-aver),2)/(2*pow(disp,2));
  double res = pow(M_E, deg) / (disp*pow(2*M_PI,0.5));
  return res;
}


//Округление с заданной точностью
// x -  [in] округляемое значение
// digits - [in] количество цифр после нуля
float roundUp( float x, int digits ){
  return floor(x*pow(10.0f,digits)+0.5f)/pow(10.0f,digits);
}

//Вычисление значения плотности вероятности в точки x
//для нормального распределения с параметрами (0.0, 1.0)
double dnorm(double x){
  double deg = -1.0*pow(x,2)/2;
  double res = pow(M_E, deg) / (1.0*pow(2*M_PI,0.5));
  return res;
}

// Вычисление вероятности (со знаком) распределения относительно начала координат
// aver - [in] математическое ожидание распределения
// disp - [in] стандартное отклонение
// a    - [in] левая граница интервала
// b    - [in] правая граница интервала
// возвращает: значение вероятности
float pnormxR32(float aver, float disp, float a, float b){
  double step = 0.01;       // шаг интегрирования
  unsigned n = 0;           // количество интервалов
  n = (unsigned)roundUp((float)((b-a)/step),0);
  double x = a;              // значение X
  double h = (b-a)/n;        // ширина столбика
  double s = 0.0f;           // общая площадь
  for (unsigned i = 1; i < n; i++){
    x += step;
    s += dnorm(x-(h/2.0),aver, disp);
  }
  s += dnorm(b-(h/2.0),aver, disp);
  s *= h;
  //Найти большую часть интеграла и установить знак по математическому ожиданию
  //if (s<0.5) s = 1.0-s;
  //s *= (aver>=0)?1.0:-1.0;
  return (float)s;
}

void pnormxR32(const float *aver, const float *disp, unsigned nelms, float a, float b, float *prob){
  for (unsigned ie=0; ie<nelms; ++ie){
    prob[ie] = pnormxR32(aver[ie],disp[ie],a,b);
  }
}

//-------------------------------------------------------------------------------------------
//  Нормальный закон распределения
//-------------------------------------------------------------------------------------------

NormDistribution::NormDistribution(){
  //Создать таблицу накопленных сумм в диапазоне от [-8.0,8.0-h]
  //(-8*1024,+8*1024)
  int    n = tbl_size;    //число шагов по всей таблице
  double h = 16.0/n;      //ширина столбца интегрирования
  tbl=new double[n];
  if (!tbl) return;

  //Заполнить значениями функции плотности вероятностей
  for (int i=0; i<n; ++i){
    double x = -8.0 + 16.0*i/n;
    tbl[i] = dnorm(x);
  }
  //Вычислить остатки
  ue = tbl[0]*h/2;  //в -oo
  oe = tbl[n-1]*h/2;//в +oo
  //Вычислить накопленную вероятность в диапазоне [-oo,x+h/2] и вычесть половину расстояния
  double s = 0.0; //накопленная сумма
  for (int i=0; i<n; ++i){
    s = s + tbl[i];
    tbl[i] = s*h - tbl[i]*h/2; //вычислить площадь без половины последнего столбика
  }
}

NormDistribution::~NormDistribution(){
  if (tbl) delete [] tbl;
}

//Значение плотности вероятности в точке x
double NormDistribution::dnorm(double x){
  double deg = -0.5*pow(x,2);
  double res = pow(M_E, deg) / (1.0*pow(2*M_PI,0.5));
  return res;
}

double NormDistribution::dnorm(double x, double aver, double disp){
  return dnorm((x-aver)/disp);
}

//Функция накопленной вероятности в интервале (-oo,x]
//для закона с параметрами (aver=0.0,disp=1.0)
double NormDistribution::cnorm(double x){
  if (x < -8.0)       return 0.0;//ue;
  if (x > 8.0)        return 1.0;//tbl.r64[tbl_size-1]+oe;
  int i = (int)((x+8.0)/16.0*tbl_size);//Вычислить индекс в таблице
  return tbl[i];
}

//Функция вероятности в интервале (a,b]
//для закона с параметрами (aver=0.0,disp=1.0)
double NormDistribution::cnorm(double a, double b){
  return cnorm(b)-cnorm(a);
}

double NormDistribution::pnorm(double x, double aver, double disp){
  //Привести к нормальному закону и вычислить для него значение
  return cnorm((x-aver)/disp);
}

double NormDistribution::pnorm(double a, double b, double aver, double disp){
  //Привести к нормальному закону и вычислить для него значение
  return cnorm((b-aver)/disp) - cnorm((a-aver)/disp);
}

//Нахождение (-oo;x) для заданной вероятности p
//примечание: для нахождения b при известно p интервала (a,b),
//нужно сложить p интервала (a,b и p интервала (-oo;a)
double NormDistribution::xnorm(double p){
  //Использование алгоритма бинарного поиска для сокращения времени поиска
  //идея: найти ближайшие к p значения и выполнить экстраполяцию
  if (p<=0.0)     return -INFINITY;
  if (p>=1.0)     return INFINITY;
  //Найти ближайшее значение в диапазоне p (0.0, 1.0)
  int ileft = 0;
  int iright= tbl_size-1;
  //Найти наиболее близкое значение к p используя алгоритм деления пополам
  //примечание: можно искать с некоторой погрешностью, тогда число последних шагов будет меньше
  while (ileft < iright){
    int i = (iright + ileft)>>1;
    if (tbl[i] >= p)  iright= i;
    else                  ileft = i+1;
  }
  if (tbl[ileft] >= p) ileft--;
  return -8.0+16.0*ileft/tbl_size;
}

double NormDistribution::xnorm(double p, double aver, double disp){
  return xnorm(p)*disp+aver;
}



//-------------------------------------------------------------------------------------------
//  Вычисление логистической функции ошибок и оценка вероятности принадлежности множеству
//-------------------------------------------------------------------------------------------


Erf::Erf(){
    //Построить таблицу
    tbl = new double[tbl_size];
    for (int i=0; i<tbl_size; ++i)
        tbl[i] = sigmath::erf(-3.0+(6.0*i)/tbl_size);
}

Erf::~Erf(){
    if (tbl) delete[] tbl;
}

double Erf::eval(double x) const{
    if (x<-3.0) return -1.0;
    if (x>=3.0) return 1.0;
    int i = tbl_size*(x+3.0)/6.0;
    return tbl[i];
}

double Erf::prob(double x, double m, double s) const{
    return eval(M_SQRT1_2*fabs(x-m)/s);
}



}
