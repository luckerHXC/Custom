#include "resizabletreeview.h"

ResizableTreeView::ResizableTreeView(QWidget* parent):QWidget(parent)
{
    setMouseTracking(true);
}
//鼠标按下触发，用于调整属性框大小
void ResizableTreeView::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::LeftButton){
    isResizing = true;
    resizeStartPos = event->pos();
    originalSize = size();
    }
    QWidget::mousePressEvent(event);

}
//鼠标移动事件可以检测鼠标是否在边界以变形
void ResizableTreeView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    if (isMouseOnBorder(mousePos)) {
        QCursor resizeCursor = QCursor(Qt::SizeHorCursor);
        setCursor(resizeCursor);
    }
    if(leavearea(mousePos)) {
        setCursor(Qt::ArrowCursor);
    }
    if (isResizing){
    int deltaX = mapFromGlobal(QCursor::pos()).x() - resizeStartPos.x();
    newWidth = originalSize.width() - deltaX;
    setMinimumWidth(newWidth);
    setMaximumWidth(newWidth);
   }

}
//鼠标释放
void ResizableTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
    isResizing = false;
    }
    QWidget::mouseReleaseEvent(event);
}
//检测是否在边界
bool ResizableTreeView::isMouseOnBorder(const QPoint& pos)
{
    // 根据需要自定义边框的区域判断条件
    QRect borderRect = QRect(0, 0, width(),height());
    return borderRect.contains(pos);
}
//离开边界
bool ResizableTreeView::leavearea(const QPoint& pos){
    QRect borderRect = QRect(6, 6, width(),height());
    return borderRect.contains(pos);
}

void ResizableTreeView::enterEvent(QEvent* event){
    setCursor(Qt::ArrowCursor);
}
