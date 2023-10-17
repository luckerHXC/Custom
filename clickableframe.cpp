#include "clickableframe.h"

ClickableFrame::ClickableFrame(QWidget* parent) : QFrame(parent)
{
    setMouseTracking(true);
}

/*实现点击发射信号*/
void ClickableFrame::mousePressEvent(QMouseEvent* event)
{
      QCursor cursor(Qt::ClosedHandCursor);
      setCursor(cursor);
      emit clicked();    // 左键点击时执行操作
}

/*实现释放发射信号*/
void ClickableFrame::mouseReleaseEvent(QMouseEvent* event)
{
      QCursor cursor(Qt::ArrowCursor);
      setCursor(cursor);
      emit released();    // 左键点击时执行操作
}
