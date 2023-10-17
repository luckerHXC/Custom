#ifndef CONTROLPROPERTY_H
#define CONTROLPROPERTY_H
#include "mainwindow.h"
#include <QObject>
#include "generate.h"
#include "ui_mainwindow.h"
#include "draggablebutton.h"
#include <QWidget>
#include "mainwindow.h"
#include <QStandardItemModel>
#include <QAbstractItemDelegate>
#include "draggableframe.h"
#include "draggableslider.h"
#include "QComboBox"
#include "clickablecombobox.h"
class MainWindow;
//此类用于将控件的属性值打印到菜单栏上
class controlproperty : public QObject
{
    Q_OBJECT
public:
    controlproperty(MainWindow * mainwindow);
    static QString newText;
    static QString firstColumnText;
    void InitTree();
    static QStandardItemModel* model;//用来存储控件数据的容器
    MainWindow * mainwindow;
    void handleTreeViewEditingFinished(QWidget* editor, QAbstractItemDelegate::EndEditHint hint);
    QPushButton* image_button;
    QPushButton* imagebackground_button;
    QPushButton* color_button;
    ClickableCombobox* backgroundcolor_combobox;
    QWidget * widget;
    void changimage();
    void changbackgroundimage();
    DraggableSlider * slider;
    QString backgroundcolor;
public slots:
    void changbackgroundcolor();
signals:
    editfinish();
    image();
    backgroundimage();

};

#endif // CONTROLPROPERTY_H
