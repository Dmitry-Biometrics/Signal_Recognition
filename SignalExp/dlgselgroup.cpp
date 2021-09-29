#include <QDebug>
#include "dlgselgroup.h"
#include "ui_dlgselgroup.h"

DlgSelGroup::DlgSelGroup(const QTreeWidget *tw, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSelGroup){
    ui->setupUi(this);

    for( int i = 0; i < tw->topLevelItemCount(); ++i ){
       const QTreeWidgetItem* from = tw->topLevelItem(i);
       QTreeWidgetItem* to = from->clone();
       markClone(from,to);
       ui->tw->addTopLevelItem(to);
    }
}

//Отметить ссылками на исходные узлы клонированное дерево
void DlgSelGroup::markClone(const QTreeWidgetItem* from, QTreeWidgetItem* to) const {
    if (!from || !to) return;
    QVariant v = qVariantFromValue((void*)from);
    to->setData(0,Qt::UserRole,v);
    for (int i=0; i<from->childCount(); ++i){
        markClone(from->child(i),to->child(i));
    }
}


DlgSelGroup::~DlgSelGroup(){
    delete ui;
}

QTreeWidgetItem* DlgSelGroup::selected() const {
    QTreeWidgetItem* item = ui->tw->currentItem();
    if (!item) return 0;
    return (QTreeWidgetItem*)item->data(0,Qt::UserRole).value<void*>();
}
