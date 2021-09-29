#ifndef DLGNNET_H
#define DLGNNET_H

#include <QDialog>
#include "sigdata.h"

namespace Ui {
class DlgNNet;
}

class DlgNNet : public QDialog
{
    Q_OBJECT

public:
    explicit DlgNNet(QWidget *parent = 0);
    ~DlgNNet();

    void setFitParams(int netType, double connPercent, int reduceRule, double q, int ycodeRule);
    void getFitParams(int &netType, double &connPercent, int &reduceRule, double &q, int &ycodeRule) const;

    void setTestParams(int testRule, const VecF32 &m, const VecF32 &s, const QVector<VecF32> &y);
    void getTestParams(int &testRule) const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DlgNNet *ui;
};

#endif // DLGNNET_H
