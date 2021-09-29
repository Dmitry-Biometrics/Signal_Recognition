#ifndef DLGSELGROUP_H
#define DLGSELGROUP_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class DlgSelGroup;
}

class DlgSelGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSelGroup(const QTreeWidget *tw, QWidget *parent = 0);
    ~DlgSelGroup();

    //Возвращает выбранный узел исходного дерева (tw)
    QTreeWidgetItem* selected() const;
protected:

    void markClone(const QTreeWidgetItem* from, QTreeWidgetItem* to) const;

private:
    Ui::DlgSelGroup *ui;
};

#endif // DLGSELGROUP_H
