#ifndef DRAGGABLEVOLUMEBAR_H
#define DRAGGABLEVOLUMEBAR_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include "QSlider"
#include "uservolumebar.h"

class DraggableVolumeBar : public QSlider
{
    Q_OBJECT
public:
    explicit DraggableVolumeBar(Qt::Orientation orientation,QWidget* parent = nullptr);
    static QList<DraggableVolumeBar*> generatedVolumeBar;
    QPoint PVolumeBar;
    QString name;
    bool signalEmitted=false;
    UserVolumeBar * volumebar;
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
    void copy(DraggableVolumeBar * slider);
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

#endif // DRAGGABLEVOLUMEBAR_H
