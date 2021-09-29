#include "dlgnnet.h"
#include "ui_dlgnnet.h"

DlgNNet::DlgNNet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgNNet)
{
    ui->setupUi(this);
}

DlgNNet::~DlgNNet()
{
    delete ui;
}

void DlgNNet::setFitParams(int netType, double connPercent, int reduceRule, double q, int ycodeRule){
    ui->cbNetType->setCurrentIndex(netType);
    ui->edConnPercent->setValue(connPercent);
    ui->cbReduceRule->setCurrentIndex(reduceRule);
    ui->edQ->setValue(q);
    ui->cbYCodeRule->setCurrentIndex(ycodeRule);
}

void DlgNNet::getFitParams(int &netType, double &connPercent, int &reduceRule, double &q, int &ycodeRule) const {
    netType     = ui->cbNetType->currentIndex();
    connPercent = ui->edConnPercent->value();
    reduceRule  = ui->cbReduceRule->currentIndex();
    q           = ui->edQ->value();
    ycodeRule   = ui->cbYCodeRule->currentIndex();
}


void DlgNNet::setTestParams(int testRule, const VecF32 &m, const VecF32 &s, const QVector<VecF32> &y){
    ui->cbTestRule->setCurrentIndex(testRule);

    for (int i=0;i<m.size(); ++i){
        const int row = ui->msy->rowCount();
        ui->msy->setRowCount(row+1);
        if (y.size()==m.size()){
            QString ycode;
            for (int j=0; j<y[i].size(); ++j)
                ycode.append(QString::number((int)y[i][j]));
            ui->msy->setItem(row, 0, new QTableWidgetItem(ycode));
        } else {
            ui->msy->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
        }
        ui->msy->setItem(row, 1, new QTableWidgetItem(QString::number(m[i])));
        ui->msy->setItem(row, 2, new QTableWidgetItem(QString::number(s[i])));
    }
}

void DlgNNet::getTestParams(int &testRule) const{
    testRule = ui->cbTestRule->currentIndex();
}




void DlgNNet::on_buttonBox_accepted(){
    accept();
}

void DlgNNet::on_buttonBox_rejected(){
    reject();
}
