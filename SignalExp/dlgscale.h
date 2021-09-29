#ifndef DLGSCALE_H
#define DLGSCALE_H

#include <QDialog>

namespace Ui {
class DlgScale;
}

class DlgScale : public QDialog
{
    Q_OBJECT

public:
    explicit DlgScale(int value, QWidget *parent = 0);
    ~DlgScale();

    int getNewValue() const;
private:
    Ui::DlgScale *ui;
};

#endif // DLGSCALE_H
