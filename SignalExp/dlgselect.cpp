#include <QDebug>
#include "dlgselect.h"
#include "ui_dlgselect.h"

#include "res.h"
#include "selrule.h"

DlgSelect::DlgSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSelect)
{
    ui->setupUi(this);

    setOperations(ui->cbOpY);
    setOperations(ui->cbOp0);
    setOperations(ui->cbOp1);
    setOperations(ui->cbOp10);

}

DlgSelect::~DlgSelect()
{
    delete ui;
}

void DlgSelect::setOperations(QComboBox *cb){
    typedef QPair<int,QString> T;
    static const QList< T > op = QList< T >()
            << T(SigMarks::Null,        STR_RES("Пустое"))
            << T(SigMarks::AnyValue,    STR_RES("Любое"))
            << T(SigMarks::Equal,       STR_RES("Равное"))
            << T(SigMarks::NotEqual,    STR_RES("Не Равное"))
            << T(SigMarks::Less,        STR_RES("Меньше"))
            << T(SigMarks::LessOrEqual, STR_RES("Не Больше"))
            << T(SigMarks::Great,       STR_RES("Больше"))
            << T(SigMarks::GreatOrEqual,STR_RES("Не Меньше"))
            << T(SigMarks::InRange,     STR_RES("В Диапазоне"))
            << T(SigMarks::NotInRange,  STR_RES("Вне Диапазона"))
            << T(SigMarks::InSet,       STR_RES("В Множестве"))
            << T(SigMarks::NotInSet,    STR_RES("Вне Множества"));
    cb->clear();
    for (int i=0; i<op.size(); ++i)
        cb->addItem(op[i].second,QVariant(op[i].first));
    cb->setCurrentIndex(1);
}


bool DlgSelect::setYDefaults(const QList<QString> &vals){//список предопределенных значений Y
    ui->edY->addItems(vals);
    return true;
}


bool DlgSelect::getRules(   SigMarks::SelRule &ruleY,
                            SigMarks::SelRule &ruleL0,
                            SigMarks::SelRule &ruleL1,
                            SigMarks::CompareOperation &op10){

    int oY = ui->cbOpY->currentData(Qt::UserRole).toInt();
    int o0 = ui->cbOp0->currentData(Qt::UserRole).toInt();
    int o1 = ui->cbOp1->currentData(Qt::UserRole).toInt();
    int o10 = ui->cbOp10->currentData(Qt::UserRole).toInt();

    VecF32 y  = stringToResp(ui->edY->currentText(),-1);
    VecF32 l0 = stringToResp(ui->edLabel0->text(),-1);
    VecF32 l1 = stringToResp(ui->edLabel1->text(),-1);

    ruleY = SigMarks::SelRule((SigMarks::CompareOperation)oY,y);
    ruleL0= SigMarks::SelRule((SigMarks::CompareOperation)o0,l0);
    ruleL1= SigMarks::SelRule((SigMarks::CompareOperation)o1,l1);
    op10  = (SigMarks::CompareOperation)o10;
    return true;
}
