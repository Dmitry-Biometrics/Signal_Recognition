#include "msg.h"
#include "res.h"
#include "sigdata.h"
#include "dlgresp.h"
#include "ui_dlgresp.h"

DlgResp::DlgResp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgResp){
    ui->setupUi(this);
}

DlgResp::~DlgResp(){
    delete ui;
}


QVector<float> DlgResp::resp() const {
    return stringToResp(ui->edResp->toPlainText(),ui->sbLength->value());
}

void DlgResp::on_btnTest_clicked(){
    if (!resp().isEmpty()){
        Msg::info(STR_RES("Значение допустимо"));
    }
}
