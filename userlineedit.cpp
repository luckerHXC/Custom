#include "userlineedit.h"
#include <QApplication>
#include <QSlider>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include "QDebug"
//用户界面的编辑框
UserLineEdit::UserLineEdit(QWidget* parent):QLineEdit(parent)
{

}

//同步值
void UserLineEdit::setintText(int value){
    this->setText(QString::number(value));
}
