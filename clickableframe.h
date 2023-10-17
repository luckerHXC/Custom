#ifndef CLICKABLEFRAME_H
#define CLICKABLEFRAME_H
#include "QFrame"

class ClickableFrame : public QFrame
{
Q_OBJECT

public:
    ClickableFrame(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    clicked();
    released();
};

#endif // CLICKABLEFRAME_H
