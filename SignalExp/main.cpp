#include "mainwindow.h"
#include "msg.h"
#include <QApplication>
#include <QDebug>

#include "sigmath.h"

int main(int argc, char *argv[])
{
    /*sigmath::NormDistribution nd;
    sigmath::Erf erf;

    double x=-3.0;
    for (int i=0; i<1000; ++i){
        //double e = sigmath::erf(x);
        double e = 1.0-erf.prob(x,0.0,1.0);
        //double e2 = sigmath::erf_prec(x,100);
        //double e3 = sigmath::erf_prec(x,1000);
        double d = sigmath::dnorm(x);
        //double c = nd.cnorm(x);
        double p = 1.0-sigmath::erf_pnorm(x,0.0,1.0);
        //dbg<<x<<"\t"<<d<<"\t"<<c<<"\t"<<p<<"\t"<<e<<'\n';
        qDebug()<<x<<"\t"<<e<<"\t"<<p<<"\t"<<d<<'\n';
        x+=0.01;
    }
    return 0;*/

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
