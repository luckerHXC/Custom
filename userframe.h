#ifndef USERFRAME_H
#define USERFRAME_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include "QFrame"
#include <QIcon>

class UserFrame : public QFrame
{
    Q_OBJECT
public:
    explicit UserFrame(QWidget* parent = nullptr);
    QPoint PFrame;
    QString savePath;
    int borderstatus;
    void setIcon(const QIcon& icon);
    void setIconSize(const QSize& size);
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event) override;
    static QList<UserFrame*> generatedUserFrame;
    //void mouseReleaseEvent(QMouseEvent* event) override;
private:
    bool isDragging = false;
    QPoint dragStartPosition;
    QIcon icon;
    QSize iconSize = QSize(16, 16);
    bool isClicked=false; // 用于记录是否点击了控件
public slots:
    void remove();//移除控件
};

#endif // USERFRAME_H
