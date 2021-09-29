#include "dlggenerate.h"
#include "ui_dlggenerate.h"
#include "rndmt.h"



DlgGenerate::DlgGenerate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgGenerate)
{
    ui->setupUi(this);
    _fixLength = -1;
}

DlgGenerate::~DlgGenerate()
{
    delete ui;
}

void DlgGenerate::setCount(int n){
    _seqCount=n;
    ui->sbCount->setValue(n);
}

void DlgGenerate::setFixLength(int n){    //предельная длина последовательности
    _fixLength = n;
    ui->sbBitSize->setValue(n);//считаем, что в примере 1 кадр (если не так, нужно менять)
}

void DlgGenerate::on_buttonBox_accepted(){
    //Настроить параметры генератора
    _seq.clear();
    QString s = ui->cbSeq->currentText();
    for (int i=0; i<s.size(); ++i){
        if      (s[i]=='0') _seq.push_back(0);
        else if (s[i]=='1') _seq.push_back(1);
        //другие значения - пропустить
    }
    if (_seq.length()==0) { reject(); return; } //недопустимая последовательность

    _level0 = ui->sbLevel0->value();
    _level1 = ui->sbLevel1->value();
    _levelNoise = ui->sbLevelNoise->value();
    _levelNoise2 = ui->sbLevelNoise2->value();

    _seed = ui->edSeed->text().toULong();
    _seed2 = ui->edSeed2->text().toULong();

    _seqCount = ui->sbCount->value();
    _bitSize = ui->sbBitSize->value();

    if (ui->cbGenReset->isChecked()){ //инициализировать явно
        _mt1.seed(_seed);
        _mt2.seed(_seed2);
    } else {                          //иначе случайным образом с использованием общего генератора
        _mt1.seed(::rand());
        _mt2.seed(::rand());
    }
    accept();
}

void DlgGenerate::on_buttonBox_rejected(){
    reject();
}


VecF32 DlgGenerate::generate(){
    int size = _bitSize*_seq.size();
    VecF32 v(size);
    for (int j=0; j<_seq.size(); ++j){  //для одного бита
        for (int i=0; i<_bitSize; ++i){ //для одного отсчета
            float x=0.0;
            if (_seq[j]==0)      x=_level0;
            else if (_seq[j]==1) x=_level1;
            //шум1
            x+=_mt1.rand(-_levelNoise,_levelNoise);   //(rand()-RAND_MAX/2);
            //шум2
            x+=_mt2.rand(-_levelNoise2,_levelNoise2); //(rand()-RAND_MAX/2);
            v[j*_bitSize+i] = x;
        }
    }
    if (_fixLength!=-1) v.resize(_fixLength);
    return v;
}



void DlgGenerate::on_cbGenReset_clicked()
{
    ui->edSeed->setEnabled(ui->cbGenReset->isChecked());
    ui->edSeed2->setEnabled(ui->cbGenReset->isChecked());
}
