#include "res.h"
#include "msg.h"
#include "dlgtrain.h"
#include "dlgresp.h"
#include "ui_dlgtrain.h"
#include "dlgselgroup.h"

DlgTrain::DlgTrain(const QTreeWidget* tw,  QWidget *parent):
    QDialog(parent),
    ui(new Ui::DlgTrain),
    _tw(tw){
    ui->setupUi(this);

    //Обнулить ссылки на данные
    for (int i=0; i<2; ++i){
        _mX[i]=0;
        _mT[i]=0;
    }

    _mY.append(VecF32());
    _mY.append(VecF32(1,0.0));
    _mY.append(VecF32(1,1.0));
    QStringList names = QStringList()<<STR_RES("авто")<<"0"<<"1";
    ui->cbY0->addItems(names);
    ui->cbY1->addItems(names);

    //Добавить список методов
    ui->cbMethod->clear();
    ui->cbMethod->addItems(_fc.types());

    //Отобразить параметры в зависимости от метода обучения
    updateParams();
    on_cbUseTestData_toggled(false);
}

void DlgTrain::updateParams(){
    _cl.clear();
    int i= ui->cbMethod->currentIndex();
    if (i<0) return;

    _cl = QSharedPointer<SigClassificator>(_fc.create(i)); //Создание классификатора

    SigClassificator::Descr d = _cl->getDescr();           //Получить описание классификатора

    QList<QWidget*> cb = QList<QWidget*>()
    << ui->edX0 << ui->tbX0 << ui->edT0 << ui->tbT0 << ui->cbY0
    << ui->edX1 << ui->tbX1 << ui->edT1 << ui->tbT1 << ui->cbY1;
    for (int g=0; g<2; ++g){    //по группам
        for (int i=0; i<5; ++i) //по элементам групп
            if (g<d.classes)  cb[g*5+i]->show();
            else              cb[g*5+i]->hide();
    }
    ui->cbUseRespData->setChecked(!d.autoY);
    ui->cbUseRespData->setEnabled(!d.autoY);

    ui->edName->setText(d.en);
}


DlgTrain::~DlgTrain(){
    delete ui;
}

QSharedPointer<SigClassificator> DlgTrain::classificator() const {
    return _cl;
}


QString DlgTrain::name() const{
    return ui->edName->text();
}

const SigMarks& DlgTrain::report() const {
    return _report;
}


void DlgTrain::on_cbUseTestData_toggled(bool checked){
    QList<QWidget*> wlist = QList<QWidget*>()
    <<ui->edT0<<ui->edT1<<ui->tbT0<<ui->tbT1;

    foreach(QWidget *w, wlist)
        w->setEnabled(checked);
}

//Добавление нового отклика
void DlgTrain::on_btnY0_clicked(){
    DlgResp dlg;
    if (dlg.exec()==QDialog::Accepted){
        VecF32 r = dlg.resp();
        if (r.isEmpty()) return;
        _mY.append(r);
        QString s = respToString(r,true);
        ui->cbY0->addItem(s);
        ui->cbY1->addItem(s);
    }
}



QVector< SigMarks* > DlgTrain::getXData() const {
    QVector< SigMarks* > x;
    for (int i=0; i<2; ++i)
        if (_mX[i]) x.push_back(_mX[i]);
        else break;
    return x;
}



QVector< SigMarks* > DlgTrain::getXTestData() const {
    QVector< SigMarks* > x;
    if (!ui->cbUseTestData->isChecked()) return x;
    for (int i=0; i<2; ++i)
        if (_mT[i]) x.push_back(_mT[i]);
        else break;
    return x;
}



QVector<VecF32> DlgTrain::getYData() const {
    QVector<VecF32> y;
    if (_fc.getDescr(ui->cbMethod->currentIndex()).autoY) return y;

    QList<QComboBox*> wlist = QList<QComboBox*>()<<ui->cbY0<<ui->cbY1;
    foreach (QComboBox* w,wlist){
        if (!w->isVisible()) break;
        int i = w->currentIndex();
        if (i<0) { y.clear(); return y; }
        y.push_back(_mY[i]);
    }
    return y;
}

bool DlgTrain::validateData(int iclass, const QVector< SigMarks* > &x, const QVector< SigMarks* > &xt, const QVector<VecF32> &y) const{
    SigClassificator::Descr d = _fc.getDescr(iclass);
    if (x.count() > d.classes ||
        xt.count()> d.classes ||
        y.count() > d.classes) { Msg::info(STR_RES("Число классов больше допустимого числа")); return false; }

    //Проверка x
    if (x.isEmpty()) { Msg::info(STR_RES("Не определены группы примеров для обучения")); return false; }

    int nfeat = -1;
    for (int g=0; g<x.count(); ++g){
        if (!x[g]->count()) { Msg::info(STR_RES("Выбрана группа, не содержащая примеров")); return false; }

        int n = (*x[g])[0].x().size();
        if (nfeat>0 && nfeat!=n) { Msg::info(STR_RES("Число параметров в групп примеров отличается (%1, %2)").arg(nfeat).arg(n)); return false; }
        nfeat = n;
    }
    if (nfeat>d.sizeX){
        Msg::info(STR_RES("Недопустимое число выходных признаков (%1, %2)").arg(nfeat).arg(d.sizeX));
        return false;
    }

    //Проверка xt, если задан
    if (!xt.isEmpty()){
        if (xt.count()!=x.count()){ Msg::info(STR_RES("Число групп для обучения и тестирования различается")); return false; }
        for (int g=0; g<xt.count(); ++g){
            if (xt[g]->count()){
                int n = (*xt[g])[0].x().size();
                if (n!=nfeat) { Msg::info(STR_RES("Число параметров в группах примеров отличается (%1, %2)").arg(nfeat).arg(n)); return false; }
            }
            //примечание: число примеров в группах может не совпадать с x
        }
    }


    //Проверка y
    if (d.autoY) return true;

    if (y.isEmpty()) { Msg::info(STR_RES("Не выбраны эталонные отклики")); return false; } //Не заданы эталонные отклики

    bool fce = d.foreignClassEnabled;
    int nresp = y[0].size();
    for (int i=0; i<y.count(); ++i){
        int n = y[0].size();
        if (i==y.count()-1 && fce && !n) continue; //В последнем классе может не быть отклика
        if (nresp!=n) { Msg::info(STR_RES("Длина откликов отличается между собой (%1, %2)").arg(nresp).arg(n)); return false; }
    }

    if (nresp>d.sizeY){
        Msg::info(STR_RES("Недопустимое число выходных параметров (%1, %2)").arg(nresp).arg(d.sizeY));
        return false;
    }

    return true;
}

void DlgTrain::redistributeData(QVector< SigMarks* > &x, QVector< SigMarks* > &xt, float prop) const{
}



//Обучение
void DlgTrain::on_btnFit_clicked(){
    qDebug()<<STR_RES("\nОбучение..");
    int iclass = ui->cbMethod->currentIndex();
    if (iclass<0 || iclass>=_fc.types().count()) return;

    //Данные перед обучением
    QVector< SigMarks* > x  = getXData();
    QVector< SigMarks* > xt = getXTestData();
    QVector<VecF32>      y  = getYData();

    //Проверить данные
    if (!validateData(iclass, x, xt, y)) return;

    //Обучить
    _cl->setName(name());
    //int dc = (_fc.foreignClassEnabled(iclass) && y.last().count()==0)? y.count()-1 : -1; //Установить значение по умолчанию, если класс "Все Чужие" разрешен
    //_cl->setDefaultClass(dc);
    //_cl->setTestPerc(ui->sbTestPerc->value()/100.0);

    if (_cl->fit(x,y)==0){ Msg::info(STR_RES("Классификатор успешно настроен")); }
    else                 { Msg::info(STR_RES("Ошибка обучения")); return; }

    //Сформировать маркированный набор для тестирования
    QVector< SigMarks* > &t = (xt.isEmpty())? x : xt;
    for (int i=0; i<t.size(); ++i){
        int pos = _report.count();//текущая позиция
        VecI32 p(t[i]->count());    //позиции
        VecI32 v(t[i]->count(),i);  //номер класса
        for (int i=0; i<p.count(); ++i) p[i] = pos+i;
        _report.append(*t[i]);
        _report.mark(0,p,v);    //маркировать обучающими данными
    }
    //Протестировать (и вычислить статистику)
    _cl->test(_report);

    //Закрыть диалог
    accept();
}

//Отмена обучения
void DlgTrain::on_btnCancel_clicked(){
    reject();
}

void DlgTrain::on_cbUseRespData_toggled(bool checked){
    if (checked){
        ui->cbY0->setEnabled(true);
        ui->cbY1->setEnabled(true);
        ui->btnY0->setEnabled(true);
    } else {
        ui->cbY0->setEnabled(false);
        ui->cbY1->setEnabled(false);
        ui->btnY0->setEnabled(false);
    }
}

void DlgTrain::on_cbMethod_currentIndexChanged(int index)
{
    updateParams();
}


//Преобразование узла дерева в имя
QString DlgTrain::groupName(const QTreeWidgetItem* w) const {
    QString name;
    while (w && !w->isHidden()){
        if (name.isEmpty()) name = w->text(0);
        else                name = w->text(0)+'-'+name;
        w = w->parent();
    }
    return name;
}

//Доступ к данным
SigMarks* DlgTrain::getMarks(const QTreeWidgetItem* w) const {
    if (!w) return 0;
    return w->data(0,Qt::UserRole).value<SigMarks*>();
}

void DlgTrain::setData(const QTreeWidgetItem *w, SigMarks **m, QLineEdit *ed){
    if (w){
        SigMarks *x = getMarks(w);
        if (x && x->count()){
            *m = x;
            ed->setText(STR_RES("%1 (%2)").arg(groupName(w)).arg(x->count()));
            return;
        }
    }
    ed->clear();
    *m = 0;
}

void DlgTrain::on_tbX0_clicked(){
    DlgSelGroup dlg(_tw);
    if (dlg.exec()!=QDialog::Accepted) return;
    setData(dlg.selected(),&_mX[0],ui->edX0);
}

void DlgTrain::on_tbX1_clicked(){
    DlgSelGroup dlg(_tw);
    if (dlg.exec()!=QDialog::Accepted) return;
    setData(dlg.selected(),&_mX[1],ui->edX1);
}

void DlgTrain::on_tbT0_clicked(){
    DlgSelGroup dlg(_tw);
    if (dlg.exec()!=QDialog::Accepted) return;
    setData(dlg.selected(),&_mT[0],ui->edT0);
}

void DlgTrain::on_tbT1_clicked(){
    DlgSelGroup dlg(_tw);
    if (dlg.exec()!=QDialog::Accepted) return;
    setData(dlg.selected(),&_mT[1],ui->edT1);
}

void DlgTrain::on_btnConfig_clicked(){
    //тут нужно работать с классификатором
    if (_cl.isNull()) return;
    _cl->config(SigClassificator::FitPage,this);
}
