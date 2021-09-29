#ifndef DLGSELECT_H
#define DLGSELECT_H

#include <QDialog>
#include <QComboBox>
#include "sigdata.h"

namespace Ui {
class DlgSelect;
}

class DlgSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelect(QWidget *parent = 0);
    ~DlgSelect();


    bool setYDefaults(const QList<QString> &vals);//список предопределенных значений Y

    bool getRules(SigMarks::SelRule &ruleY, SigMarks::SelRule &ruleL0, SigMarks::SelRule &ruleL1, SigMarks::CompareOperation &op10);
private:
    Ui::DlgSelect *ui;
    void setOperations(QComboBox* cb);
};

#endif // DLGSELECT_H
