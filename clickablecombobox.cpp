#include "clickablecombobox.h"

ClickableCombobox::ClickableCombobox(QWidget* parent): QComboBox(parent)
{
    setMouseTracking(true);
}


void ClickableCombobox::mousePressEvent(QMouseEvent *event){
    emit clicked();
    QComboBox::mousePressEvent(event);
}
