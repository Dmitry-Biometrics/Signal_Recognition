#include "dlgselrange.h"
#include "ui_dlgselrange.h"

DlgSelRange::DlgSelRange(int maxcount,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSelRange),
    _maxcount(maxcount)
{
    ui->setupUi(this);
    ui->lbCount->setText(QString::number(maxcount));

    ui->edStart->setMinimum(-maxcount);
    ui->edStart->setMaximum(maxcount-1);
    ui->edStart->setValue(0);

    ui->edStep->setMinimum(-maxcount);
    ui->edStep->setMaximum(maxcount);
    ui->edStep->setValue(1);

    ui->edCount->setMinimum(-maxcount);
    ui->edCount->setMaximum(maxcount);
    ui->edCount->setValue(maxcount);

    ui->edPercent->setMinimum(0.0);
    ui->edPercent->setMaximum(100.0);
    ui->edPercent->setValue(0.0);
}

DlgSelRange::~DlgSelRange()
{
    delete ui;
}

bool DlgSelRange::getRange(int &start, int &step, int &count) const{
    start = ui->edStart->value();
    if (start<0) start+=_maxcount;

    step = ui->edStep->value();

    if (ui->edPercent->value()!=0.0){
        count = (int)(_maxcount*ui->edPercent->value()/100.0);
    } else {
        count = ui->edCount->value();
        if (count<0) count+=_maxcount+1;
    }
    return true;
}

void DlgSelRange::on_buttonBox_accepted(){ accept();}
void DlgSelRange::on_buttonBox_rejected(){ reject();}
