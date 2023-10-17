#ifndef CONNECTSL_H
#define CONNECTSL_H
#include "mainwindow.h"
#include <QObject>
#include "generate.h"
#include "ui_mainwindow.h"
#include "draggablebutton.h"
#include <QWidget>
#include "mainwindow.h"
#include "controlproperty.h"
#include "draggablelabel.h"
#include "draggablelineedit.h"
#include "draggablecheckbox.h"
#include "draggableradiobutton.h"
#include "draggableslider.h"
#include "draggabletoolbutton.h"
#include "draggableframe.h"
class MainWindow;
class connectSL: public QObject
{
    Q_OBJECT
public:
    connectSL(MainWindow * parent);
    MainWindow * mainwidow;
    void eventfiter(QWidget *widget);
};

#endif // CONNECTSL_H
