#include "userframe.h"
#include "draggableframe.h"
#include "controlproperty.h"
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QDomDocument>
#include <QTimer>
#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QMimeData>
#include <QDrag>
#include "widgethelper.h"
#include <QAbstractButton>
//用户界面的背景板
QList<UserFrame*> UserFrame::generatedUserFrame;
UserFrame::UserFrame(QWidget* parent)
    :  QFrame(parent)
{
    // 更新边线颜色
    setStyleSheet("QFrame { background-color: transparent;}");

}

void UserFrame::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);

    if (!icon.isNull())
    {
        QPainter painter(this);
        painter.drawPixmap(5, 5, icon.pixmap(iconSize));
    }
    // 如果点击了控件，则绘制绿色边框
    if (isClicked==true)
    {
        QPainter p(this);
        p.setPen(QColor("green"));
        p.drawRect(0, 0, width() - 1, height() - 1);
    }

}
void UserFrame::setIcon(const QIcon& icon)
{
    this->icon = icon;
    update();
}

void UserFrame::setIconSize(const QSize& size)
{
    iconSize = size;
    update();
}

// 在需要改变鼠标指针形状的控件上设置悬停事件
void UserFrame::enterEvent(QEvent* event)
{
    Q_UNUSED(event);
    // 设置鼠标指针形状为手型
    setCursor(Qt::PointingHandCursor);

}

// 在鼠标离开控件时恢复默认形状
void UserFrame::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    unsetCursor();
}

void UserFrame::mousePressEvent(QMouseEvent* event)
{
    // 鼠标点击事件发生在控件内，设置为点击状态
    if (rect().contains(event->pos()))
    {
        for(UserFrame* uf:generatedUserFrame){
            uf->isClicked=false;
            uf->update();
        }
        isClicked = true;
        update(); // 触发重绘

    }

}

void UserFrame::remove(){
    generatedUserFrame.removeOne(this);
}



