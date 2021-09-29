#ifndef DLGTRAIN_H
#define DLGTRAIN_H

#include <QDialog>
#include <QTreeWidget>
#include "sigdata.h"
#include "sigfactory.h"

namespace Ui {
class DlgTrain;
}

class DlgTrain : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTrain(const QTreeWidget* tw, QWidget *parent=0);
    ~DlgTrain();

    QSharedPointer<SigClassificator> classificator() const; //классификатор
    const SigMarks& report() const;                         //отчет по результата обучения
    QString name() const;

    void updateParams();    //обновить параметры обучения в зависимости от выбранного классификатора

private slots:
    void on_cbUseTestData_toggled(bool checked);
    void on_btnY0_clicked();
    void on_btnFit_clicked();

    void on_btnCancel_clicked();
    void on_cbUseRespData_toggled(bool checked);

    void on_cbMethod_currentIndexChanged(int index);

    void on_tbX0_clicked();

    void on_tbX1_clicked();

    void on_tbT0_clicked();

    void on_tbT1_clicked();

    void on_btnConfig_clicked();

protected:
    QVector< SigMarks* > getXData() const;
    QVector< SigMarks* > getXTestData() const;
    QVector<VecF32> getYData() const;

    bool validateData(int iclass, const QVector< SigMarks* > &x, const QVector< SigMarks* > &xt, const QVector<VecF32> &y) const;
    void redistributeData(QVector< SigMarks* > &x, QVector< SigMarks* > &xt, float prop) const;
    QString     groupName(const QTreeWidgetItem* item) const;
    SigMarks*   getMarks(const QTreeWidgetItem* w) const;

    void setData(const QTreeWidgetItem *w, SigMarks **m, QLineEdit *ed);
private:
    Ui::DlgTrain *ui;
    const QTreeWidget* _tw;

    SigFactory _fc;         //фабрика методов обучения
    SigMarks* _mX[2];       //обучающие данные (по классам)
    SigMarks* _mT[2];       //тестовые данные
    QVector<VecF32> _mY;    //допустимые отклики (нет, 0, 1, xFFFF, xFF00)
    QStringList     _mYNames;

    QSharedPointer<SigClassificator> _cl;//активный классификатор
    SigMarks _report;                    //отчет с результатами тестирования
};

#endif // DLGTRAIN_H
