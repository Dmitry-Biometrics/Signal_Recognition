#ifndef DLGNORM_H
#define DLGNORM_H

#include <QDialog>

namespace Ui {
class DlgNorm;
}

class DlgNorm : public QDialog
{
    Q_OBJECT

public:
    explicit DlgNorm(QWidget *parent = 0);
    ~DlgNorm();

    float getMinY() const;
    float getMaxY() const;

private:
    Ui::DlgNorm *ui;
};

#endif // DLGNORM_H
