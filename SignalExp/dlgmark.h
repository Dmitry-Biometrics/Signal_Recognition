#ifndef DLGMARK_H
#define DLGMARK_H

#include <QDialog>

namespace Ui {
class DlgMark;
}

class DlgMark : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMark(QWidget *parent = 0);
    ~DlgMark();

    bool enabledL0() const;
    bool enabledL1() const;
    int getL0() const;
    int getL1() const;

private slots:
    void on_chL0_toggled(bool checked);
    void on_chL1_toggled(bool checked);

private:
    Ui::DlgMark *ui;
};

#endif // DLGMARK_H
