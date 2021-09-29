#ifndef DLGSELRANGE_H
#define DLGSELRANGE_H

#include <QDialog>

namespace Ui {
class DlgSelRange;
}

class DlgSelRange : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelRange(int maxcount, QWidget *parent = 0);
    ~DlgSelRange();

    bool getRange(int &start, int &step, int &count) const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DlgSelRange *ui;
    int _maxcount;
};

#endif // DLGSELRANGE_H
