#include "connectsl.h"
#include "ui_mainwindow.h"
connectSL::connectSL(MainWindow * parent)
{
     this->mainwidow=parent;
}

void connectSL::eventfiter(QWidget *widget){
    widget->installEventFilter(mainwidow);
}
