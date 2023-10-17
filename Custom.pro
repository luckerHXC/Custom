#-------------------------------------------------
#
# Project created by QtCreator 2023-07-04T14:29:43
#
#-------------------------------------------------
QT       += widgets
QT       += core gui xml
QT       += websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Custom
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    draggablebutton.cpp \
    generate.cpp \
    savebutton.cpp \
    printcontroll.cpp \
    controlproperty.cpp \
    mytreeview.cpp \
    resizabletreeview.cpp \
    widgethelper.cpp \
    draggabledial.cpp \
    draggablelabel.cpp \
    clickablelabel.cpp \
    draggablelineedit.cpp \
    clickablelineedit.cpp \
    draggablecheckbox.cpp \
    draggableradiobutton.cpp \
    draggableslider.cpp \
    draggabletoolbutton.cpp \
    draggableframe.cpp \
    userframe.cpp \
    userslider.cpp \
    userlineedit.cpp \
    clickableframe.cpp \
    userdial.cpp \
    draggablecommandlinkbutton.cpp \
    draggablevolumebar.cpp \
    uservolumebar.cpp \
    clickablecombobox.cpp

HEADERS += \
        mainwindow.h \
    draggablebutton.h \
    generate.h \
    savebutton.h \
    printcontroll.h \
    controlproperty.h \
    mytreeview.h \
    resizabletreeview.h \
    widgethelper.h \
    draggabledial.h \
    draggablelabel.h \
    clickablelabel.h \
    draggablelineedit.h \
    clickablelineedit.h \
    draggablecheckbox.h \
    draggableradiobutton.h \
    draggableslider.h \
    draggabletoolbutton.h \
    draggableframe.h \
    userframe.h \
    userslider.h \
    userlineedit.h \
    clickableframe.h \
    userdial.h \
    draggablecommandlinkbutton.h \
    draggablevolumebar.h \
    uservolumebar.h \
    clickablecombobox.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    C:/Users/RDSE/Pictures/1.jpg \
    C:/Users/RDSE/Pictures/2.jpg \
    C:/Users/RDSE/Pictures/1690254681101_5854580B-9315-4937-8B81-C5EAC52E8B20.png \
    C:/Users/RDSE/Pictures/1690267611171_20635CCB-0259-4d37-8B7D-0842A2733C9F.png \
    C:/Users/RDSE/Pictures/slider.png
