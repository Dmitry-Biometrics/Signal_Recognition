#include "dlgmark.h"
#include "ui_dlgmark.h"

DlgMark::DlgMark(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMark)
{
    ui->setupUi(this);
}

DlgMark::~DlgMark()
{
    delete ui;
}

void DlgMark::on_chL0_toggled(bool checked){
    ui->sbL0->setEnabled(checked);
}

void DlgMark::on_chL1_toggled(bool checked){
    ui->sbL1->setEnabled(checked);
}

bool DlgMark::enabledL0() const {
    return ui->chL0->isChecked();
}

bool DlgMark::enabledL1() const {
    return ui->chL1->isChecked();
}

int DlgMark::getL0() const {
    return ui->sbL0->value();
}

int DlgMark::getL1() const {
    return ui->sbL1->value();
}
