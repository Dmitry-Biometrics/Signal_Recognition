#ifndef DLGXI2_H
#define DLGXI2_H

#include <QDialog>

namespace Ui {
class DlgXi2;
}

class DlgXi2 : public QDialog
{
    Q_OBJECT

public:
    explicit DlgXi2(QPair<double,double> cur, QWidget *parent = 0);
    ~DlgXi2();

    QPair<double,double> value() const;

private:
    Ui::DlgXi2 *ui;
};

#endif // DLGXI2_H
