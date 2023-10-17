#ifndef PRINTCONTROLL_H
#define PRINTCONTROLL_H
#include "mainwindow.h"
#include <QObject>
#include "generate.h"
#include "ui_mainwindow.h"
#include "draggablebutton.h"
#include "draggablecheckbox.h"
#include <QWidget>
#include "draggableradiobutton.h"
#include "draggabletoolbutton.h"
#include "draggableframe.h"
#include "draggableslider.h"
#include "draggablelineedit.h"
#include "userframe.h"
#include "userslider.h"
#include "userdial.h"
#include "draggablecommandlinkbutton.h"
#include "draggablevolumebar.h"
class MainWindow;

class printcontroll : public QObject
{
public:
    printcontroll(MainWindow * parent);
    MainWindow * mainwindow;
    template<typename T,typename U>
    void printall_t(QString filepath, QList<T*>& generatedControls,QString name);
    template<typename T,typename U>
    void printall_not(QString filepath, QList<T*>& generatedControls,QString name);
    void printslider();
    void printToolButton();
    void printFrame();
    void printLineEdit();
    void printvolumebar();
    void layers();
    QMap<QWidget*, int> rankMap;
    QMap<QWidget*, QWidget*> controlMap;
public slots:
    void printall();
    void updatecontroll();
};

#endif // PRINTCONTROLL_H
