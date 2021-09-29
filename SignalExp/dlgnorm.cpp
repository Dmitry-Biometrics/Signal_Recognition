#include "dlgnorm.h"
#include "ui_dlgnorm.h"

DlgNorm::DlgNorm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgNorm)
{
    ui->setupUi(this);
}

DlgNorm::~DlgNorm()
{
    delete ui;
}

float DlgNorm::getMinY() const {
    return ui->sbMin->value();
}

float DlgNorm::getMaxY() const {
    return ui->sbMax->value();
}
