#ifndef DLGGENERATE_H
#define DLGGENERATE_H

#include <QDialog>
#include "sigdata.h"
#include "rndmt.h"

namespace Ui {
class DlgGenerate;
}

class DlgGenerate : public QDialog
{
    Q_OBJECT

public:
    explicit DlgGenerate(QWidget *parent = 0);
    ~DlgGenerate();

    int count()   const { return _seqCount; }
    int bitSize() const { return _bitSize; }
    int length()  const { return _bitSize*_seq.size(); }   //длина одного примера

    void setCount(int n);
    void setFixLength(int n);    //предельная длина последовательности

    VecF32 generate();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_cbGenReset_clicked();

private:
    Ui::DlgGenerate *ui;

    VecI32 _seq;    //последовательность
    int _bitSize;   //число отсчетов в одном кадре
    int _seqCount;  //число генерируемых примеров
    int _fixLength;


    float _level0;      //уровень 0
    float _level1;      //уровень 1
    float _levelNoise;  //уровень шума [-levelNoise;+levelNoise]
    float _levelNoise2; //уровень шума

    unsigned _seed;          //посев
    unsigned _seed2;

    RndMT _mt1;
    RndMT _mt2;
};

#endif // DLGGENERATE_H
