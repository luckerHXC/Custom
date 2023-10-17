#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QTreeView>
//此类是为了让treeview的第一列禁止被编辑
class MyTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MyTreeView(QWidget* parent = nullptr);

protected:
    bool edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) override;
};

#endif // MYTREEVIEW_H
