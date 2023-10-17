#ifndef DRAGGABLEDIAL_H
#define DRAGGABLEDIAL_H
#include "QDial"
#include <QMouseEvent>
#include <QVBoxLayout>
#include "userdial.h"

class DraggableDial: public QDial
{
    Q_OBJECT
    Q_PROPERTY(QString styleSheet READ styleSheet WRITE setstyleSheet)
public:
    explicit DraggableDial(QWidget* parent = nullptr);
    static QList<DraggableDial*> generatedDials;
    QPoint PDial;
    UserDial * control;
    QString name;
    bool signalEmitted=false;
    int matchId;
    int getmatchID();
    void setMatchId(int id);
    int imageCounter;
    QString imagePath;
    QString savePath;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    QPixmap backgroundImage();
    void setBackgroundImage(QPixmap pixmap);
    void paintEvent(QPaintEvent *event);
    void drawTunerKnob(QPainter *painter);
    QString styleSheet();
    void setstyleSheet(const QString &styleSheet);
    double angleStep;
    bool drawblue=false;
    void copy(DraggableDial * dial);
    int frameid;
    int layers;
    bool lock=true;


protected:
    void mousePressEvent(QMouseEvent* event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event) override;//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event) override;//松开鼠标事件
    void showContextMenu(const QPoint& pos);//菜单栏展示
    void wheelEvent(QWheelEvent* event);//缩放
    void Tree();
    void drawBlueBorder(int borderWidth);


public slots:
    void remove();//移除控件
    void changeproperty();
    void clicktree();
    void changimage();
    void changbackgroundimage();
signals:
    void clicked();
private:
    QString m_dialStyleSheet;

};

#endif // DRAGGABLEDIAL_H
