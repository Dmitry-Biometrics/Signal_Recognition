#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QSignalMapper>
#include <qcustomplot.h>
#include <sigdata.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void  log(const QString &text);//вывод сообщения
private slots:
    void on_btnAddSamples_clicked();
    void on_tw_itemSelectionChanged();
    void on_btnInvSelectedItem_clicked();
    void on_btnDelSamples_clicked();
    void on_lw_currentRowChanged(int currentRow);
    void on_btnShowCurve_clicked();
    void on_btnShowLog_clicked();
    void on_menuSel(int);
    void on_btnAddDir_clicked();
    void on_btnExportSamples_clicked();
    void on_btnTrain_clicked();
    void on_btnTest_clicked();
    void on_btnAddGroup_clicked();
    void on_btnDelGroup_clicked();
    void on_btnMark_clicked();

    void on_menuUnOp(int);
    void on_menuBinOp(int);
    void on_mnPrScale();
    void on_mnFurie();
    void on_mnPrNorm();
    void on_mnPrMean();
    void on_mnEnergy();

    void on_mnAddNoise();

    void on_pushButton_clicked();
    void on_btnCopySamples_clicked();
    void on_tbLockCurve_clicked();
    void on_tbClearCurve_clicked();
    void on_tbColor_clicked();
    void on_tbOnTop_clicked();
    void on_tbOnBottom_clicked();
    void on_btnValue_clicked();
    void on_btnClearLog_clicked();


    void on_mnUniteYAndCopyToX();
    void on_mnUniteLb0AndCopyToX();
    void on_mnUniteLb1AndCopyToX();

    void on_btnClassifConfig_clicked();

    void on_btnGenerate_clicked();

    void on_log(const QString &text);
protected:
    virtual void 	closeEvent(QCloseEvent *event);

    QMenu* createMenuMapping(const QStringList &names, QSignalMapper *mapper);

    void lockView();
    void unlockView();

    bool isRootGroup(const QTreeWidgetItem* group, const QString& name) const;
    void updateList();
    void setGroupCount(int n);

    SigMarks* curMarks(const QString &rootName = QString(), bool createIfNotExits=false) const;
    SigMarks* getMarks(QTreeWidgetItem* w, bool createIfNull=true);
    QVector<VecF32*> curData() const;

    QTreeWidgetItem* curGroup() const;
    QTreeWidgetItem* findGroup(const QString &name) const;
    QTreeWidgetItem* addGroup(const QString &name, bool isNested);
    QTreeWidgetItem* addGroup(QTreeWidgetItem* parent, const QString &name, SigMarks *m, bool timestamp);

    QVector<int> curSel() const;    //текущий список выделенных примеров


    QMenu* menuSelectByLabel();

    //QList<QTreeWidgetItem*>  groupList(const QString &name) const;          //Список узлов, имеющих в корне имя name
    //QList<QTreeWidgetItem*>  groupList(const QTreeWidgetItem* item) const;  //Список узлов, имеющих в качестве родителя item
    //QStringList  groupNames(const QList<QTreeWidgetItem*> &items) const;    //Преобразование элементов к списку полных имен
    //QList<SigMarks*> groupData(const QList<QTreeWidgetItem*> &items) const; //Список данных

    void  clearCurve();
    void  addCurve(const VecF32 &y, QPen pen, bool autoAxes); //Добавление кривой на диаграмму


    QString floatToString(const QVector<float> &data) const;


    //Добавление гистограммы для длины кода n разрядов
    //@param x   целочисленный вектор мер Хэмминга
    //@param count длина вектора
    //@param n   длина кода (максимальное значение)
    //@return прогноз ошибки второго рода
    /*float addHammHistI32(const Vbp &x, int count, int n, QBrush brush, QPen pen, float &a, float &d);
    //Добавление кривой на диаграмму
    void  addCurve(int32_t count, Vbp &x, Vbp &y, QPen pen);
    //Добавление распределения откликов на график
    //(добавляются отдельные точки и их аппроксимирующее распределение)
    void addDistr(int index, const QVector<double> &x, QPen pen, const QString &legend, double aver=0.0, double disp=-1.0);
    //Добавление нормального распределения
    void addNormCurve(double aver, double disp, QPen pen);*/

    QMenu* createSelMenu();
    QMenu* createUnOpMenu();
    QMenu* createBinOpMenu();

    void logReport(const QString &name, const SigMarks &m);

    void setColorBox(QLabel *label, QColor c);


    void uniteAndCopy(int what);


private:
    Ui::MainWindow *ui;
    QList<QSharedPointer<SigClassificator> > _cl;

    static const QStringList _unOpNames;
    static const QStringList _binOpNames;
    bool _stop;

    struct {
        QColor                   color;
        QCPAbstractPlottable*    cur;
        int                      pos;
        QList<QCPAbstractPlottable*> locked;
    } _curve;
};

#endif // MAINWINDOW_H
