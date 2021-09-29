#include "dlgnewgroup.h"
#include "ui_dlgnewgroup.h"

DlgNewGroup::DlgNewGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgNewGroup)
{
    ui->setupUi(this);
}

DlgNewGroup::~DlgNewGroup()
{
    delete ui;
}

QString DlgNewGroup::getGroupName() const {
    return ui->edGroupName->text();
}

bool    DlgNewGroup::isNested() const{
    return ui->cbChild->isChecked();
}
