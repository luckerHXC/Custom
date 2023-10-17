#include "MyTreeView.h"
//重写树类，用于当做属性框
MyTreeView::MyTreeView(QWidget* parent) : QTreeView(parent)
{
    // 设置编辑触发方式为双击
    setEditTriggers(QAbstractItemView::DoubleClicked);
}
//可编辑
bool MyTreeView::edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event)
{
    // 如果是第一列，不进行编辑
    if (index.column() == 0)
    {
        return false;
    }

    return QTreeView::edit(index, trigger, event);
}
