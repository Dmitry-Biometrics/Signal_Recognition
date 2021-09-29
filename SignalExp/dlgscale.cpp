#include "dlgscale.h"
#include "ui_dlgscale.h"

DlgScale::DlgScale(int value, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgScale){
    ui->setupUi(this);
    ui->lblValue->setText(QString::number(value));
    ui->sbNewValue->setValue(value);
}

DlgScale::~DlgScale(){
    delete ui;
}

int DlgScale::getNewValue() const {
    return ui->sbNewValue->value();
}
