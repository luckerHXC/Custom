#include "clickablelabel.h"
#include <QMouseEvent>
#include "QDebug"
/*标签重写*/
ClickableLabel::ClickableLabel(QWidget* parent) : QLabel(parent)
{
    setMouseTracking(true);
}

/*实现点击发射信号*/
void ClickableLabel::mousePressEvent(QMouseEvent* event)
{

    if (event->button() == Qt::LeftButton){
      emit clicked();    // 左键点击时执行操作
    }

    QLabel::mousePressEvent(event);

}
