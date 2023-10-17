#ifndef GENERATE_H
#define GENERATE_H
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
#include "draggablecommandlinkbutton.h"
class MainWindow;
class generate: public QObject
{
    Q_OBJECT

public:
    generate(MainWindow * parent);
    MainWindow * mainwindow;
    template<typename T>
    T* generateControl();


public slots:
     void generateButton();
     void generateDial();
     void generateLabel();
     void generateLineEdit();
     void generateCheckBox();
     void generateRadioButton();
     void generatedhorizontalSlider();
     void generatedverticalSlider();
     void generatedToolButton();
     void generatedFrame();
     void generatedCommandLinkButton();
     void generatedVolumeBar();
};

#endif // GENERATE_H
