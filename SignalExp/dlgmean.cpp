#include "dlgmean.h"
#include "ui_dlgmean.h"

DlgMean::DlgMean(int nsamp, int nfeat, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMean)
{
    ui->setupUi(this);
    ui->lblN->setText(QString::number(nsamp));
    ui->lblNFeat->setText(QString::number(nfeat));
}

DlgMean::~DlgMean()
{
    delete ui;
}

int DlgMean::getWindowLength() const{
    return ui->sbWindow->value();
}

int DlgMean::getStep() const{
    return ui->sbStep->value();
}

bool DlgMean::isMeanChecked() const{
    return ui->chMean->isChecked();
}

bool DlgMean::isStdevChecked() const{
    return ui->chStdev->isChecked();
}

bool DlgMean::isQualChecked() const{
    return ui->chQual->isChecked();
}
