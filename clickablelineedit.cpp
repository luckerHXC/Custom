#include "clickablelineedit.h"
#include <QMouseEvent>
/*编辑框重写*/
ClickableLineEdit::ClickableLineEdit(QWidget* parent) : QLineEdit(parent)
{
   setMouseTracking(true);
   this->setReadOnly(true);  // 设置编辑框为不可编辑
   setStyleSheet("border: 2px solid black;");


}

/*实现点击发射信号*/
void ClickableLineEdit::mousePressEvent(QMouseEvent* event)
{

    if (event->button() == Qt::LeftButton){
      emit clicked();    // 左键点击时执行操作
    }

    QLineEdit::mousePressEvent(event);

}
