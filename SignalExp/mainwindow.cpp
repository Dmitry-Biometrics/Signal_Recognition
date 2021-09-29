#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msg.h"
#include "res.h"
#include "sigdata.h"
#include "sigmath.h"
#include "dlgtrain.h"
#include "dlgnewgroup.h"
#include "dlgselect.h"
#include "dlgmark.h"
#include "dlgscale.h"
#include "dlgnorm.h"
#include "dlgselgroup.h"
#include "dlgmean.h"
#include "dlgselrange.h"
#include "dlggenerate.h"

void MainWindow::on_menuSel(int id){
    SigMarks *m = curMarks();
    if (!m){ Msg::info(S_GROUP_NOT_SELECTED); return; }
    QVector<int> cur = curSel();//упорядоченный список выбранных элементов (если есть)
    if (cur.count() && !Msg::confirm(STR_RES("Искать в ранее выделенных?")))
        cur.clear();

    SigMarks::SelRule ruleY = SigMarks::SelRule(SigMarks::AnyValue,0.0);
    SigMarks::SelRule rule0;
    SigMarks::SelRule rule1;
    SigMarks::CompareOperation rule10 = SigMarks::AnyValue;

    int start, step, count;

    switch (id){
    case 0: //"с номерами"
        {
            DlgSelRange dlg((cur.size())?cur.size():m->count());
            if (dlg.exec()!=QDialog::Accepted || !dlg.getRange(start,step,count)) return;
            break;
        }
    case 1: //"новые" //-1 (-1)
        rule0 = SigMarks::equal(-1);
        rule1 = SigMarks::equal(-1);
        break;
    case 2: //"немаркированные" -1 (любой)
        rule0 = SigMarks::anyValue();
        rule1 = SigMarks::equal(-1);
        break;
    case 3: //"маркированные"  не -1 (любой)
        rule0 = SigMarks::anyValue();
        rule1 = SigMarks::notEqual(-1);
        break;
    case 4: //"эталонные" любой (не -1)
        rule0 = SigMarks::notEqual(-1);
        rule1 = SigMarks::anyValue();
        break;
    case 5: //"совпадающие" "свой" x=x, не -1
        rule0 = SigMarks::notEqual(-1);
        rule1 = SigMarks::notEqual(-1);
        rule10 = SigMarks::Equal;
        break;
    case 6: //"с ошибками" x!=x, не -1
        rule0 = SigMarks::notEqual(-1);
        rule1 = SigMarks::notEqual(-1);
        rule10 = SigMarks::NotEqual;
        break;
    default:
        {
            DlgSelect dlg;
            if (m){  //Сформировать список предопределенных значений Y (если есть)
                QMap<QString,int> map;
                QList<QString> yvals;
                if (!m->_c.isNull()){
                    foreach(const VecF32& v, m->_c->y())
                        map[respToString(v,true)]=1;
                }
                for (int i=0; i<m->count(); ++i){
                    QString s = respToString(m->at(i).y(),true);
                    map[s]=1;
                    if (map.count()>20) break; //Принудительно остановить
                }
                yvals = map.keys();
                dlg.setYDefaults(yvals);
            }
            if (dlg.exec()!=QDialog::Accepted || !dlg.getRules(ruleY,rule0,rule1,rule10))
                return;
        }
    }

    VecI32 sel;
    if (id==0) sel = m->select(start,count,step,cur.count() ? &cur : 0);
    else       sel = m->select(ruleY, rule0, rule1, rule10, cur.count() ? &cur : 0);
    //Изменить выделение объектов по условию
    ui->lw->clearSelection();
    for (int i=0; i<sel.count(); ++i){
        ui->lw->item(sel[i])->setSelected(true);
    }
}

QMenu* MainWindow::createSelMenu(){
    QStringList names = QStringList()
    << "с номерами"     //start step count
    << "новые"          //-1 (-1)
    << "немаркированные"//-1 (любой)
    << "маркированные"  //не -1 (любой)
    << "эталонные"      //любой (не -1)
    << "совпадающие"    //x=x, не -1
    << "с ошибками"     //x!=x, не -1
    << "другие..";      //диалог

    QSignalMapper *mapper = new QSignalMapper(this);
    QMenu *menu = new QMenu();
    for (int i=0; i<names.count(); ++i){
        QAction* action = new QAction(names[i],menu);
        menu->addAction(action);
        mapper->setMapping(action,i);
        connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(on_menuSel(int)));
    return menu;
}

QMenu* MainWindow::createMenuMapping(const QStringList &names, QSignalMapper *mapper){
    QMenu *menu = new QMenu();
    for (int i=0; i<names.count(); ++i){
        QAction* action = new QAction(names[i],menu);
        menu->addAction(action);
        mapper->setMapping(action,i);
        connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
    }
    return menu;
}

const QStringList MainWindow::_unOpNames = QStringList()
        <<"neg"    //Neg,
        <<"abs"    //Abs,
        <<"int"    //Int,
        <<"frac"   //Frac,
        <<"round"  //Round,
        <<"exp"    //Exp,
        <<"pow2"   //Pow2,
        <<"log2"   //Log2,
        <<"log"    //LogE,
        <<"log10"  //Log10,
        <<"hev"    //Hev,
        <<"sin"    //Sin,
        <<"cos"    //Cos,
        <<"tan"    //Tan,
        <<"asin"   //ASin,
        <<"acos"   //ACos,
        <<"atan";  //ATan

const QStringList MainWindow::_binOpNames = QStringList()
        <<"add"    //Add,
        <<"sub"    //Sub,
        <<"mul"    //Mul,
        <<"div"    //Div,
        <<"pow";   //Pow,

QMenu* MainWindow::createUnOpMenu(){
    QSignalMapper *mapper = new QSignalMapper(this);
    QMenu* menu = createMenuMapping(_unOpNames, mapper);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(on_menuUnOp(int)));
    return menu;
}

QMenu* MainWindow::createBinOpMenu(){
    QSignalMapper *mapper = new QSignalMapper(this);
    QMenu* menu = createMenuMapping(_binOpNames, mapper);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(on_menuBinOp(int)));
    return menu;
}


void MainWindow::on_menuUnOp(int id){
    QVector<VecF32*> x = curData();
    if (x.isEmpty()){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }
    addGroup(curGroup(),_unOpNames[id],new SigMarks(sigmath::unOp((sigmath::UnOp)id,x)),true);
    log(STR_RES("\nФункция %1 применена к %2 примерам").arg(_unOpNames[id]).arg(x.count()));
}

void MainWindow::on_menuBinOp(int id){
    //x,c    введите константу
    //x,y[0] выберите вектор
    //x,y    выберите каталог
    log(QString::number(id));
}




void MainWindow::on_mnPrScale(){
    QVector<VecF32*> x = curData();
    if (x.isEmpty()){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }

    DlgScale dlg(x[0]->size());
    if (dlg.exec()!=QDialog::Accepted) return;

    SigMarks *u = new SigMarks();
    u->append(sigmath::interp(x,dlg.getNewValue()));
    addGroup(curGroup(),STR_RES("scale"),u,true);
    log(STR_RES("\nФункция scale применена к %2 примерам").arg(u->count()));
}

void MainWindow::on_mnFurie(){
    QVector<VecF32*> x = curData();
    if (x.isEmpty()){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }

   DlgScale dlg(x[0]->size());
//    if (dlg.exec()!=QDialog::Accepted) return;

    SigMarks *u = new SigMarks();
  //  u->append(sigmath::interp(x,dlg.getNewValue()));
    u->append(sigmath::get_furie(x));
    addGroup(curGroup(),STR_RES("Furie"),u,true);
    log(STR_RES("\nФункция фурье применена к %2 примерам").arg(u->count()));
}

void MainWindow::on_mnPrNorm(){
    QVector<VecF32*> x = curData();
    if (x.isEmpty()){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }

    DlgNorm dlg;
    if (dlg.exec()!=QDialog::Accepted) return;

    SigMarks *u = new SigMarks();
    u->append(sigmath::norm(x,dlg.getMinY(),dlg.getMaxY()));
    addGroup(curGroup(),STR_RES("norm"),u,true);
    log(STR_RES("\nФункция norm применена к %2 примерам").arg(u->count()));
}


void MainWindow::on_mnPrMean() {
    QVector<VecF32*>    x = curData();
    if (x.isEmpty()){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }

    int n = x.count();

    DlgMean dlg(n,x[0]->size());
    if (dlg.exec() != QDialog::Accepted) return;

    int size = dlg.getWindowLength();   if (size<=0) size = n;
    int step = dlg.getStep();           if (step<=0) step = 1;

    int n2 = (n-size+1)/step;
    if (n2<=0) { Msg::info(STR_RES("Недопустимые размеры окна (n2=%1, size=%2, step=%3)").arg(n2).arg(size).arg(step)); return; }  //Неправильно выбран размер окна

    log(STR_RES("\nФункции mean, stdev, qual применены к %1 примерам (число элементов %2, окно %3, шаг %4)").arg(n).arg(n2).arg(size).arg(step));

    bool flag_mean = dlg.isMeanChecked();
    bool flag_std  = dlg.isStdevChecked();
    bool flag_qual = dlg.isQualChecked();

    QVector<VecF32> xm; xm.reserve(n2);
    QVector<VecF32> xs; xs.reserve(n2);
    QVector<VecF32> xq; xq.reserve(n2);

    for (int i=0; i<(n-size+1); i+=step){
        if (flag_mean || flag_std || flag_qual) xm.push_back(sigmath::mean(x,i,size));
        if (flag_std || flag_qual)              xs.push_back(sigmath::stdev(x,xm.last(),i,size));
        if (flag_qual)                          xq.push_back(sigmath::qual(xm.last(),xs.last()));
    }

    if (flag_mean)  addGroup(curGroup(),STR_RES("mean"),new SigMarks(xm),true);
    if (flag_std)   addGroup(curGroup(),STR_RES("std"),new SigMarks(xs),true);
    if (flag_qual)  addGroup(curGroup(),STR_RES("qual"),new SigMarks(xq),true);
}

//Вычисление энергии по выборке
void MainWindow::on_mnEnergy(){
    QVector<VecF32*>    x = curData();
    if (x.isEmpty()){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }

    log(STR_RES("\nФункция энергии применена к %1 примерам").arg(x.size()));
    float e = sigmath::energy(x);
    log(STR_RES("энергия = %1").arg(e));
}

void MainWindow::on_mnAddNoise(){
    //Сигнал
    QVector<VecF32*>    x = curData();
    if (x.isEmpty()){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }
    int n = x.count();

    DlgGenerate dlg;
    dlg.setCount(x.size());     //число примеров
    dlg.setFixLength(x[0]->size());//общая длина сигнала (общая длина не должна выходить за этот размер, т.е. можно делать resize(_maxlength))

    if (dlg.exec() != QDialog::Accepted) return;
    lockView();
    float es = sigmath::energy(x);  //энергия сигнала
    QVector<VecF32> y(x.size());
    for (int i=0; i<n; ++i) y[i]=dlg.generate();
    float en = sigmath::energy(y);  //энергия шума (среднеквадратичное амплитуды)
    float sn = (en==0.0)?1e30:es/en;   //отношение ск амплитуд
    float sn_db = 20*::log10(sn);   //20 - для амплитуд; 10 - для мощностей
    //получить смешанный сигнал
    for (int i=0; i<n; ++i) sigmath::set_add(y[i],*x[i]);
    addGroup(curGroup(),STR_RES("mix"),new SigMarks(y),true);

    log(STR_RES("\nПолучена смесь сигналов и шума для %1 примеров").arg(x.size()));
    log(STR_RES("среднеквадр. амплидуды сигнала= %1\n амплитуды шума= %2\nотношение s/n= %3 (%4 dB)").arg(es).arg(en).arg(sn).arg(sn_db));

    unlockView();
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _stop(false)
{
    ui->setupUi(this);
    connect(this,SIGNAL(log(QString)),this,SLOT(on_log(QString)));

    QFont consolasFont("Consolas", 8);
    ui->log->setFont(consolasFont);

    _curve.color = QColor(Qt::green);
    _curve.cur = 0;
    _curve.pos = 0;
    setColorBox(ui->lbColor,_curve.color);

    ui->mnUnOp2->setMenu(createUnOpMenu());
    ui->mnBinOp->setMenu(createBinOpMenu());

    ui->btnSelectSamples->setMenu(createSelMenu());
    //ui->btnCopySamples->setPopupMode(QToolButton::InstantPopup);

    connect(ui->mnPrScale,SIGNAL(triggered()),this,SLOT(on_mnPrScale()));
    connect(ui->mnFurie,SIGNAL(triggered()),this,SLOT(on_mnFurie()));
    connect(ui->mnPrNorm,SIGNAL(triggered()),this,SLOT(on_mnPrNorm()));
    connect(ui->mnPrMean,SIGNAL(triggered()),this,SLOT(on_mnPrMean()));
    connect(ui->mnGenSeq,SIGNAL(triggered()),this,SLOT(on_btnGenerate_clicked()));
    connect(ui->mnEnergy,SIGNAL(triggered()),this,SLOT(on_mnEnergy()));
    connect(ui->mnAddNoise,SIGNAL(triggered()),this,SLOT(on_mnAddNoise()));

    connect(ui->mnUniteYAndCopyToX,SIGNAL(triggered()),this,SLOT(on_mnUniteYAndCopyToX()));
    connect(ui->mnUniteLb0AndCopyToX,SIGNAL(triggered()),this,SLOT(on_mnUniteLb0AndCopyToX()));
    connect(ui->mnUniteLb1AndCopyToX,SIGNAL(triggered()),this,SLOT(on_mnUniteLb1AndCopyToX()));

    connect(ui->mnInverse,SIGNAL(triggered()),this,SLOT(on_btnInvSelectedItem_clicked()));
    connect(ui->mnMark,SIGNAL(triggered()),this,SLOT(on_btnMark_clicked()));


    ui->tw->setFocus();

    ui->lw->setUniformItemSizes(true);

    //Добавление специальных клавиш в панели

    QShortcut* shKeyMul = new QShortcut(QKeySequence(Qt::Key_Plus),this);
    connect(shKeyMul, SIGNAL(activated()), this, SLOT(on_btnAddSamples_clicked()));
    QShortcut* shKeyDelete = new QShortcut(QKeySequence(Qt::Key_Delete),this);
    connect(shKeyDelete, SIGNAL(activated()), this, SLOT(on_btnDelSamples_clicked()));
    QShortcut* shKeySlash = new QShortcut(QKeySequence(Qt::Key_Slash),this);
    connect(shKeySlash, SIGNAL(activated()), this, SLOT(on_btnInvSelectedItem_clicked()));
    Msg::installHandler(this);
}




void  MainWindow::on_log(const QString &text){
    qApp->processEvents();
    ui->log->appendPlainText(text);
}

MainWindow::~MainWindow(){
    Msg::installHandler(0);
    delete ui;
}


bool MainWindow::isRootGroup(const QTreeWidgetItem* group, const QString& rootName) const{
    if (!group) return 0;
    while (group->parent()) group=group->parent();
    return (group->text(0)==rootName);
}


SigMarks* MainWindow::curMarks(const QString &rootName, bool createIfNotExist) const {
    QList<QTreeWidgetItem*> sel = ui->tw->selectedItems();
    if (sel.count()!=1 || (!rootName.isEmpty() && !isRootGroup(sel[0],rootName)))
        return 0;
    SigMarks* m = sel[0]->data(0,Qt::UserRole).value<SigMarks*>();
    if (!m && createIfNotExist) sel[0]->setData(0,Qt::UserRole,QVariant().fromValue(m = new SigMarks()));
    return m;
}

QVector<VecF32*> MainWindow::curData() const {
    SigMarks *m = curMarks();
    if (!m) return QVector<VecF32*>();

    QVector<int> p = curSel();
    if (p.count()) return m->x(p);
    else           return m->x();
}


QVector<int> MainWindow::curSel() const{    //текущий список выделенных примеров
    QVector<int> v;
    QListWidget* lw = ui->lw;
    v.reserve(lw->count());
    for (int i=0; i<lw->count(); ++i)
        if (lw->item(i)->isSelected()){
            v.push_back(i);
        }
    return v;
}


void MainWindow::updateList(){
    clearCurve();
    ui->plot->replot();
    ui->lw->clear();
    SigMarks* m = curMarks();
    if (!m) return;
    for (int i=0; i<m->count(); ++i){
        QString name = STR_RES("%1(%2)| %3").arg(m->at(i)._lb[1]).arg(m->at(i)._lb[0]).arg((*m)[i].name());
        ui->lw->addItem(name);
    }
}



void MainWindow::setGroupCount(int n){
    QList<QTreeWidgetItem*> sel = ui->tw->selectedItems();
    if (sel.count()!=1) return;
    sel[0]->setText(1,QString("%1").arg(n));
}



QTreeWidgetItem* MainWindow::curGroup() const {
    return ui->tw->currentItem();
}

QTreeWidgetItem* MainWindow::findGroup(const QString &name) const {
    QTreeWidgetItem* root = ui->tw->invisibleRootItem();
    for (int i=0; i<root->childCount(); ++i){
        if (root->child(i)->text(0) == name){
            return root->child(i);
        }
    }
    return 0;
}

QTreeWidgetItem* MainWindow::addGroup(QTreeWidgetItem* parent, const QString &name, SigMarks *m, bool timestamp){
    if (!m) m = new SigMarks();
    if (!parent) parent = ui->tw->invisibleRootItem();
    QTreeWidgetItem* item = new QTreeWidgetItem();
    if (timestamp){
        QString stamp = QTime::currentTime().toString("hhmmss");
        item->setText(0,name+'-'+stamp);
    } else {
        item->setText(0,name);
    }
    item->setData(0,Qt::UserRole,QVariant().fromValue(m));
    item->setText(1,QString::number(m->count()));
    parent->addChild(item);
    return item;
}

void MainWindow::lockView(){
    ui->tw->setEnabled(false);
    ui->lw->setEnabled(false);
}

void MainWindow::unlockView(){
    ui->tw->setEnabled(true);
    ui->lw->setEnabled(true);
}

//Добавление примеров
void MainWindow::on_btnAddSamples_clicked(){
    SigMarks* m = curMarks(QString(),true);
    if (!m){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }
    //Дополнить данными
    QStringList paths = QFileDialog::getOpenFileNames(this, STR_RES("Файлы данных"), QDir::currentPath(), STR_RES("Все (*.txt *.xlsx *.bin);;Тестовые файлы (*.txt);;Табличные файлы (*.xlsx);;Бинарные файлы (*.bin)"));
    log(STR_RES("Добавление примеров из файлов"));
    lockView();//запретить изменять во время длительной загрузки
    m->reserve(m->count()+paths.count());
    bool ok=false;
    for (int i=0; i<paths.count(); ++i){
        qApp->processEvents();
        QFileInfo fi(paths[i]);
        if (m->appendFromFile(fi.path(),fi.fileName())) log(STR_RES("..%1 ок").arg(fi.fileName()));
        else                                            log(STR_RES("..%1 ошибка").arg(fi.fileName()));
        if (_stop){ _stop=false; break; }
        ok=true;
    }
    if (ok){
        setGroupCount(m->count());
        updateList();
    }
    unlockView();
}



void MainWindow::on_btnAddDir_clicked(){
    SigMarks* m = curMarks(S_DATA_GROUP,true);
    if (!m){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }
    //Добавить из каталога
    QStringList filter = QStringList() << "*.txt" << "*.xlsx" << "*.bin";
    QString dir = QFileDialog::getExistingDirectory(this, STR_RES("Каталог данных"),QDir::currentPath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDirIterator it(dir, filter, QDir::Files, QDirIterator::NoIteratorFlags);
    log(STR_RES("Добавление примеров из каталога %1").arg(dir));
    //Подсчитать число файлов
    int n=0;
    while (it.hasNext()){ it.next(); ++n; }
    m->reserve(m->count()+n);
    //Добавить файлы
    lockView();//запретить изменять каталог во время длительной загрузки
    bool ok=false;
    QDirIterator it2(dir, filter, QDir::Files, QDirIterator::NoIteratorFlags);
    while (it2.hasNext()){
        qApp->processEvents();
        QFileInfo fi(it2.next());
        if (m->appendFromFile(fi.path(),fi.fileName())) log(STR_RES("..%1 ок").arg(fi.fileName()));
        else                                            log(STR_RES("..%1 ошибка").arg(fi.fileName()));
        if (_stop){ _stop=false; break; }
        ok=true;
    }
    if (ok){
        setGroupCount(m->count());
        updateList();
    }
    unlockView();
}



void MainWindow::on_btnExportSamples_clicked(){
    SigMarks *m = curMarks();
    if (!m){ Msg::info(S_GROUP_NOT_SELECTED); return; }
    QVector<int> p = curSel();
    if (p.isEmpty()){ //Добавить все примеры, если не выбраны
        for (int i=0; i<m->count(); ++i) p.push_back(i);
    }
    if (Msg::confirm("Сохранить в одном файле?")){
        //Сохранить в общем файле
        QString path = QFileDialog::getSaveFileName(this,STR_RES("Сохранение в едином файле"),QDir::currentPath(),STR_RES("Бинарные файлы (*.bin)"));
        if (path.isEmpty()) return;
        log(STR_RES("Экспорт данных"));
        lockView();
        if (m->writeToBinFile(path,p))    log(STR_RES("..ок"));
        else                              log(STR_RES("..ошибка"));
        unlockView();

    } else {
        //Сохранить с отдельными именами
        QString path = QFileDialog::getSaveFileName(this,STR_RES("Сохранение в отдельных файлых"),QDir::currentPath(),STR_RES("Тестовые файлы (*.txt)"));
        if (path.isEmpty()) return;
        log(STR_RES("Экспорт данных"));
        QString ext = path.split('.').last();
        path = path.left(path.length()-1-ext.count());
        lockView();
        for (int i=0; i<p.count(); ++i){
            if (m->writeToTxtFile(path+QString::number(p[i])+'.'+ext,p[i]))
                log(STR_RES("..%1 ок").arg(i));
            else
                log(STR_RES("..%1 ошибка").arg(i));
        }
        unlockView();
    }
}


void MainWindow::on_tw_itemSelectionChanged(){
    updateList();
}

//Инверсия выделения
void MainWindow::on_btnInvSelectedItem_clicked(){
    log(STR_RES("Инверсия примеров"));
    int count = ui->lw->count();
    for (int i=0; i<count; ++i){
        QListWidgetItem* w = ui->lw->item(i);
        if (w->isSelected())    w->setSelected(false);
        else                    w->setSelected(true);
        if (!count&0x007F) QApplication::processEvents();//обработать сообщения
    }
    //при большом числе элементом зависает
    //log(STR_RES("Все выделены"));
    //ui->lw->selectAll();
}


//Удаление выделения
void MainWindow::on_btnDelSamples_clicked(){
    SigMarks* m = curMarks();
    if (!m) return; //группа не выбрана
    QList<QListWidgetItem*> items = ui->lw->selectedItems();
    if (!items.count()){
        if (!Msg::confirm(STR_RES("Удалить все элементы?")))
            return;
        m->clear();
    } else {
        if (!Msg::confirm(STR_RES("Удалить выделенные элементы?")))
            return;
        QVector<int> pos;
        for (int i=0; i<ui->lw->count(); ++i){
            if (!ui->lw->item(i)->isSelected()) continue;
            pos.push_back(i);
        }
        m->remove(pos);
    }
    log(STR_RES("Удаление примеров"));
    setGroupCount(m->count());
    updateList();
}




void MainWindow::on_btnShowCurve_clicked(){
    QList<QWidget*> list = QList<QWidget*>()
    <<ui->plot<<ui->chPlotFix<<ui->tbClearCurve<<ui->tbLockCurve
    <<ui->lbColor<<ui->tbColor;
    bool show = ui->btnShowCurve->isChecked();
    foreach(QWidget* x, list)
        (show) ? x->show():x->hide();
}

void MainWindow::on_btnShowLog_clicked(){
    if (ui->btnShowLog->isChecked())    ui->log->show();
    else                                ui->log->hide();
}


void MainWindow::on_btnTrain_clicked(){
    DlgTrain dlg(ui->tw);
    if (QDialog::Accepted==dlg.exec()){
        _cl.append(dlg.classificator());
        SigClassificator::Descr d = _cl.last()->getDescr();

        SigMarks *m = new SigMarks(dlg.report());
        QString name = dlg.name();
        QTreeWidgetItem * g = addGroup(findGroup(STR_RES("обучение")),name,m,true);
        ui->cbClassif->addItem(g->text(0));
        ui->cbClassif->setCurrentIndex(ui->cbClassif->count()-1);
        logReport(g->text(0),*m);
    }
}


//Тестирование
void MainWindow::on_btnTest_clicked(){
    int iclass = ui->cbClassif->currentIndex();
    if (iclass<0){ Msg::info(STR_RES("Классификатор не выбран/не создан")); return; }

    SigMarks* m = curMarks();
    if (!m) { Msg::info(S_GROUP_NOT_SELECTED); return; }
    QVector<int> p = curSel();

    //Сформировать список отчета
    SigMarks* u = (p.count())? new SigMarks(m->slice(p)): new SigMarks(*m);

    QSharedPointer<SigClassificator> cl = _cl[iclass];
    SigClassificator::Descr d = cl->getDescr();
    u->classif(cl);
    cl->test(*u);
    //Добавить в дерево
    QString title = STR_RES("тест-%1").arg(cl->name());
    addGroup(curGroup(),title,u,true);
    //Вывести статистику
    logReport(title,*u);
    Msg::info(STR_RES("Тестирование выполнено успешно"));
}


void MainWindow::logReport(const QString &name, const SigMarks &m){
    QMap<int, QMap<int,int> > r = m.stats();
    log(STR_RES("\nСтатистика по набору %1:").arg(name));
    int count0 = m.count();  //общее число элементов
    foreach(int k0, r.keys()){
        int count1 = 0; //число элементов в классе 0
        foreach(int v1, r[k0].values())
            count1 += v1;
        foreach (int k1, r[k0].keys()){
            int x = r[k0][k1];
            float p0 = (float)x/count0*100.0; //от общее числа
            float p1 = (float)x/count1*100.0;//от эталонного класса
            QString text = STR_RES("%1=>%2: %3 из %4 (%5); %6\% от класса '%1'; %7\% от набора")
                    .arg(k0).arg(k1).arg(x).arg(count1).arg(count0).arg(p1).arg(p0);
            log(text);
        }
    }
}

QTreeWidgetItem* MainWindow::addGroup(const QString &name, bool isNested){
    if (name.isEmpty()) return 0;
    QTreeWidgetItem *root = ui->tw->currentItem();
    if (!isNested) root = root->parent();
    if (!root) root = ui->tw->invisibleRootItem();
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0,name);
    root->addChild(item);
    return item;
}

void MainWindow::on_btnAddGroup_clicked(){
    DlgNewGroup dlg;
    if (dlg.exec()!=QDialog::Accepted) return;
    addGroup(dlg.getGroupName(),dlg.isNested());
}

void MainWindow::on_btnDelGroup_clicked(){
    if (Msg::confirm(STR_RES("Удалить выделенную группу и все вложенные?"))){
        QList<QTreeWidgetItem*> items = ui->tw->selectedItems();
        for (int i=0; i<items.count(); ++i){
            SigMarks* m = items[i]->data(0,Qt::UserRole).value<SigMarks*>();
            if (m) delete m;
            delete items[i];
        }
    }
}

void MainWindow::on_btnMark_clicked(){
    //Получить текущую группу
    SigMarks* m = curMarks();
    QVector<int> sel = curSel();
    if (!m){ Msg::info(S_GROUP_NOT_SELECTED); return; }
    if (!sel.count() && !Msg::confirm(STR_RES("Маркировать все примеры группы?"))) return;

    DlgMark dlg;
    if (dlg.exec()!=QDialog::Accepted) return;
    if (sel.count()){
        if (dlg.enabledL0()) { VecI32 val(sel.count(),dlg.getL0()); m->mark(0,sel,val); }
        if (dlg.enabledL1()) { VecI32 val(sel.count(),dlg.getL1()); m->mark(1,sel,val); }
    } else {
        if (dlg.enabledL0()) { VecI32 val(m->count(),dlg.getL0()); m->mark(0,val); }
        if (dlg.enabledL1()) { VecI32 val(m->count(),dlg.getL1()); m->mark(1,val); }
    }
    updateList();
}

void MainWindow::on_pushButton_clicked(){
    DlgSelGroup dlg(ui->tw);
    if (dlg.exec()!=QDialog::Accepted) return;
    QTreeWidgetItem *item = dlg.selected();
    if (item)   log(item->text(0));
    else        log("NULL");
}


SigMarks* MainWindow::getMarks(QTreeWidgetItem* w, bool createIfNull){
    if (!w) return 0;
    SigMarks* m = w->data(0,Qt::UserRole).value<SigMarks*>();
    if (!m && createIfNull) w->setData(0,Qt::UserRole,QVariant().fromValue(m = new SigMarks()));
    return m;
}


void MainWindow::on_btnCopySamples_clicked()
{
    SigMarks *m = curMarks();       //текущая группа
    QVector<int> p= curSel();       //текущие выделенные примеры
    if (!m){ Msg::info(S_GROUP_NOT_SELECTED); return; }
    if (!p.count() && !Msg::confirm(STR_RES("Выполнить операцию над всеми записями?"))) return;

    DlgSelGroup dlg(ui->tw);
    if (dlg.exec()!=QDialog::Accepted || !dlg.selected()) return;
    QTreeWidgetItem *item = dlg.selected();
    SigMarks *m2 = getMarks(item,true);

    if (p.count()) m2->append(m->slice(p));
    else           m2->append(*m);
    item->setText(1,QString::number(m2->count()));
}

void 	MainWindow::closeEvent(QCloseEvent *event){
    _stop=true;
    QMainWindow::closeEvent(event);
}

QString MainWindow::floatToString(const QVector<float> &v) const{
    QString s;
    for(int i=0; i<v.size()-1; i++)
        s += QString::number(v[i])+',';
    if (v.size()) s+= QString::number(v.last());
    return s;
}

void MainWindow::on_lw_currentRowChanged(int currentRow){
    clearCurve();

    if (currentRow<0) return; //если нет выбранного
    //Показать график, если нужно
    SigMarks* m = curMarks();
    if (!m || currentRow >= m->count()) return;
    //Показать данные примера label0, label1, resp и его график
    SigMark &r = (*m)[currentRow];
    ui->txtLabel->setText(STR_RES("Класс: %1 (%2)").arg((int)r._lb[1]).arg((int)r._lb[0]));
    ui->txtY->setText(STR_RES("Отклик: %1").arg(floatToString(r._y)));

    addCurve(r.x(),_curve.color, !ui->chPlotFix->isChecked());
}


//Добавление кривой на диаграмму
void MainWindow::addCurve(const VecF32 &y, QPen pen, bool autoAxes){
    QCustomPlot* plot = ui->plot;
    QCPCurve *curve = new QCPCurve(plot->xAxis, plot->yAxis);
    plot->addPlottable(curve);
    _curve.cur = curve;

    int count = y.size();
    QVector<double> X(count), Y(count);
    for (int i=0; i<count; ++i){ X[i]=i; Y[i]=y[i];} //*1000; }
    curve->setData(X,Y);
    curve->setPen(pen);

    // set some basic plot config:
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom ); //| QCP::iSelectPlottables);
    plot->axisRect()->setupFullAxesBox();
    if (autoAxes) plot->rescaleAxes();

    ui->plot->movePlottable(plot->plottableCount()-1,_curve.pos);
    ui->plot->replot();
}



void MainWindow::clearCurve(){
    if (_curve.cur) ui->plot->removePlottable(_curve.cur);
    _curve.cur=0;
}



//Заблокировать все имеющиеся графики
void MainWindow::on_tbLockCurve_clicked(){
    if (_curve.cur && !_curve.locked.contains(_curve.cur)){
        if (_curve.pos==_curve.locked.count()) _curve.pos++;
        _curve.locked.push_front(_curve.cur);
        QColor color = QColorDialog::getColor(_curve.color,this,STR_RES("Цвет фикс. графика"));
        _curve.cur->setPen(QPen(color));
        _curve.cur=0;
        ui->plot->replot();
    }
}

void MainWindow::on_tbClearCurve_clicked(){
    foreach(QCPAbstractPlottable* c, _curve.locked)
        ui->plot->removePlottable(c);
    _curve.locked.clear();
    _curve.pos = 0;
    clearCurve();
    ui->plot->replot();
}


void MainWindow::setColorBox(QLabel *label, QColor c){
    QPixmap pixmap(16, 16);
    pixmap.fill(c);
    label->setPixmap(pixmap);
}

void MainWindow::on_tbColor_clicked(){
    _curve.color = QColorDialog::getColor(_curve.color,this,STR_RES("Цвет новых графиков"));
    setColorBox(ui->lbColor,_curve.color);

    if (_curve.cur) _curve.cur->setPen(QPen(_curve.color));
    ui->plot->replot();
}


//Перемещение заблокированных графиков наверх
void MainWindow::on_tbOnTop_clicked(){
    if (!_curve.cur) return;
    int old = _curve.pos;
    _curve.pos++;
    if (_curve.pos>_curve.locked.count()) _curve.pos=_curve.locked.count();

    ui->plot->movePlottable(old,_curve.pos+1);
    ui->plot->replot();
}

void MainWindow::on_tbOnBottom_clicked(){
    if (!_curve.cur) return;
    int old = _curve.pos;
    _curve.pos--;
    if (_curve.pos<0) _curve.pos=0;

    ui->plot->movePlottable(old,_curve.pos);
    ui->plot->replot();
}

void MainWindow::on_btnValue_clicked(){
    SigMarks *m = curMarks();
    QVector<int> p = curSel();
    if (!m || p.isEmpty()) {
        Msg::info(STR_RES("Не выделен пример"));
        return;
    }

    log("\n");
    for (int i=0; i<p.count(); ++i){
        QString s = respToString((*m)[i].x(),false);
        log(s);
    }
}

void MainWindow::on_btnClearLog_clicked(){
    ui->log->clear();
}

void MainWindow::on_mnUniteYAndCopyToX(){   uniteAndCopy(1); }
void MainWindow::on_mnUniteLb0AndCopyToX(){ uniteAndCopy(2); }
void MainWindow::on_mnUniteLb1AndCopyToX(){ uniteAndCopy(3); }

void MainWindow::uniteAndCopy(int what){
    SigMarks* m = curMarks();
    VecI32 p = curSel();
    if (!m){ Msg::info(S_GROUP_NOT_SELECTED); return; }
    if (!p.count()){
        if (!Msg::confirm(STR_RES("Выполнить операцию над всеми записями?"))) return;
        else {
            p.resize(m->count());
            for (int i=0; i<m->count(); ++i) p[i]=i;
        }
    }

    QStringList name = QStringList()<<"unite-x"<<"unite-y"<<"unite-lb0"<<"unite-lb1";

    SigMarks *u = new SigMarks();
    switch (what){
    case 0: u->append(sigmath::unite(m->x(p))); break;
    case 1: u->append(sigmath::unite(m->y(p))); break;
    case 2: u->append(sigmath::convI32ToF32(m->label(0))); break;
    case 3: u->append(sigmath::convI32ToF32(m->label(1))); break;
    default: return;
    }

    addGroup(curGroup(),name[what],u,true);
}


void MainWindow::on_btnClassifConfig_clicked(){
    //настроить текущий классификатор
    int i = ui->cbClassif->currentIndex();
    if (i<0) return;

    QSharedPointer<SigClassificator> c = _cl[i];
    c->config(SigClassificator::TestPage,this);
}

//Генерация последовательностей с заданным уровнем шума
void MainWindow::on_btnGenerate_clicked(){
    SigMarks* m = curMarks(S_DATA_GROUP,true);
    if (!m){ Msg::info(S_DATAGROUP_NOT_SELECTED); return; }

    DlgGenerate dlg;
    if (dlg.exec() != QDialog::Accepted) return;
    int n = dlg.count();    //число примеров
    m->reserve(m->count()+n);
    //Добавить последовательности
    bool ok=false;
    for (int i=0; i<n;++i){
        m->append(dlg.generate());
        qApp->processEvents();
        if (_stop){ _stop=false; break; }
        ok=true;
    }
    if (ok){ setGroupCount(m->count()); updateList(); }
}
