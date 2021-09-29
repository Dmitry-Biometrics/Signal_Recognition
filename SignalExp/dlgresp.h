#ifndef DLGRESP_H
#define DLGRESP_H

#include <QDialog>

namespace Ui {
class DlgResp;
}

class DlgResp : public QDialog
{
    Q_OBJECT

public:
    explicit DlgResp(QWidget *parent = 0);
    ~DlgResp();

    QVector<float> resp() const;
    int charToHex(char c) const;

private slots:
    void on_btnTest_clicked();

private:
    Ui::DlgResp *ui;

    int test();//0 - хорошо, иначе, число недостающих бит

};

#endif // DLGRESP_H
