#ifndef WIDGETHELPER_H
#define WIDGETHELPER_H
#include"QWidget"
#include <QList>
#include "draggablebutton.h"
#include "draggabledial.h"
#include "draggableframe.h"
#include "draggablelabel.h"
#include "draggablelineedit.h"
#include "draggablecheckbox.h"
#include "draggableradiobutton.h"
#include "draggableslider.h"
#include "draggabletoolbutton.h"
#include "QGraphicsDropShadowEffect"
#include "draggablecommandlinkbutton.h"
#include "draggablevolumebar.h"
class MainWindow;
class WidgetHelper: public QObject
{


public:
    WidgetHelper(MainWindow * parent);
    static MainWindow * mainwindow;
    static void mousePressEvent(QMouseEvent* event,QWidget *widget);//通用鼠标点击
    static void mouseMoveEvent(QMouseEvent* event,QWidget *widget);//通用鼠标移动
    static void mouseReleaseEvent(QMouseEvent* event,QWidget *widget);//通用鼠标释放
    static void wheelEvent(QWheelEvent* event,QWidget *widget);//通用控件缩放
    static void clicktree();//控件标识
    static void tree(QWidget *widget);//数据打包并准备给属性展示框
    static void changeproperty(QWidget *widget);//更改属性框的数值以后可以影响
    static void showContextMenu(const QPoint& pos,QWidget *widget);//菜单栏展示
    static void showContextMenufortab(const QPoint& pos,QWidget *widget);
    static void handleContextMenuAction(QAction *action);
    static QPoint m_dragStartPosition;
    static bool delet;
    static bool isResizing;
    static QString resizeCorner;
    static QPoint resizeStartPosition;
    static QSize originalSize;
    static QString name;
    static QString Text;
    static QList<QWidget*> copiedWidgets;
    static void findLeftmostWidget(const QList<QWidget*>& widgetList);
    static QMap<QWidget*, QPoint> distancesMap;
    static bool enable;
    static void cancelborder();
    static void raiseORdown(QWidget * control,QString UD);
    static bool checklock(QWidget * control);
    static void printblue(QWidget * widget,bool blue);



public slots:

private:
    static DraggableButton * button;
    static DraggableDial * dial;
    static DraggableCheckBox * checkbox;
    static DraggableFrame * frame;
    static DraggableLabel * label;
    static DraggableLineEdit * lineEdit;
    static DraggableRadioButton * radiobutton;
    static DraggableSlider * slider;
    static DraggableToolButton * toolbutton;
    static DraggableCommandLinkButton * commandlinkbutton;
    static DraggableVolumeBar * volumebar;



};

#endif // WIDGETHELPER_H
