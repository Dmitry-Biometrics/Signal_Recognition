#ifndef DLGNEWGROUP_H
#define DLGNEWGROUP_H

#include <QDialog>

namespace Ui {
class DlgNewGroup;
}

class DlgNewGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DlgNewGroup(QWidget *parent = 0);
    ~DlgNewGroup();

    QString getGroupName() const;
    bool    isNested() const;

private:
    Ui::DlgNewGroup *ui;
};

#endif // DLGNEWGROUP_H
