#ifndef DRAGGABLESLIDER_H
#define DRAGGABLESLIDER_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include "QSlider"
#include "userslider.h"
class DraggableSlider : public QSlider
{
    Q_OBJECT
public:
    explicit DraggableSlider(Qt::Orientation orientation,QWidget* parent = nullptr);
    static QList<DraggableSlider*> generatedSlider;
    QPoint PSlider;
    QString name;
    bool signalEmitted=false;
    UserSlider * uslider= nullptr;
    int matchId;
    void setMatchId(int id);
    int getmatchID();
    QString savePath;
    QString imagePath;
    int imageCounter;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    void paintEvent(QPaintEvent *event);
    bool drawblue=false;
    void copy(DraggableSlider * slider);
    void changcolor();
    int frameid;
    int layers;
    bool lock=true;
protected:
    void mousePressEvent(QMouseEvent* event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event) override;//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event) override;//松开鼠标事件
    void showContextMenu(const QPoint& pos);//菜单栏展示
    void wheelEvent(QWheelEvent* event);//缩放
    void addImage();
    void Tree();
public slots:
    void remove();//移除控件
    void changeproperty();
    void clicktree();
    void changimage();
    void changbackgroundimage();
signals:
    void clicked();
};

#endif // DRAGGABLESLIDER_H
