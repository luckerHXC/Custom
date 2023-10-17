#ifndef DRAGGABLERADIOBUTTON_H
#define DRAGGABLERADIOBUTTON_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include "QRadioButton"
class DraggableRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit DraggableRadioButton(QWidget* parent = nullptr);
    static QList<DraggableRadioButton*> generateRadioButton;
    QPoint PRadioButton;
    bool signalEmitted=false;
    int matchId;
    QString name;
    int getmatchID();
    void setMatchId(int id);
    int imageCounter;
    QString imagePath;
    QString savePath;
    QRadioButton* control;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    void copy(DraggableRadioButton * radiobutton);
    int frameid;
    int layers;
    bool lock=true;
    bool drawblue=false;
protected:
    void mousePressEvent(QMouseEvent* event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event) override;//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event) override;//松开鼠标事件
    void showContextMenu(const QPoint& pos);//菜单栏展示
    void wheelEvent(QWheelEvent* event);//缩放
    void Tree();
    void paintEvent(QPaintEvent* event) override;
public slots:
    void remove();//移除控件
    void changeproperty();
    void clicktree();
    void changimage();
    void changbackgroundimage();
signals:
    void clicked();
};

#endif // DRAGGABLERADIOBUTTON_H
