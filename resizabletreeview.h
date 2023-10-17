#ifndef RESIZABLETREEVIEW_H
#define RESIZABLETREEVIEW_H
#include <QtWidgets>

class ResizableTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit ResizableTreeView(QWidget* parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool isMouseOnBorder(const QPoint& pos);
    bool leavearea(const QPoint& pos);
     void enterEvent(QEvent* event) override;
private:
    QVBoxLayout *layout;
    QTreeView *treeView;
    bool isResizing = false;
    QPoint resizeStartPos;
    QSize originalSize;
    int newWidth;

};

#endif // RESIZABLETREEVIEW_H
