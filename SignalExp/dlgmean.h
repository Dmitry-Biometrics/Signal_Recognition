#ifndef DLGMEAN_H
#define DLGMEAN_H

#include <QDialog>

namespace Ui {
class DlgMean;
}

class DlgMean : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMean(int nsamp, int nfeat, QWidget *parent = 0);
    ~DlgMean();

    int getWindowLength() const;
    int getStep() const;

    bool isMeanChecked() const;
    bool isStdevChecked() const;
    bool isQualChecked() const;

private:
    Ui::DlgMean *ui;
};

#endif // DLGMEAN_H
