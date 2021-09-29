#include "dlgxi2.h"
#include "ui_dlgxi2.h"

DlgXi2::DlgXi2(QPair<double,double> cur, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgXi2)
{
    ui->setupUi(this);

    ui->sbMin->setValue(cur.first);
    ui->sbMax->setValue(cur.second);
}

QPair<double,double> DlgXi2::value() const {
    QPair<double,double> r;
    r.first = ui->sbMin->value();
    r.second= ui->sbMax->value();
    return r;
}


DlgXi2::~DlgXi2()
{
    delete ui;
}
