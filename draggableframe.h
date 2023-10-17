#ifndef DRAGGABLEFRAME_H
#define DRAGGABLEFRAME_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include "QFrame"
#include <QIcon>
class DraggableFrame : public QFrame
{
    Q_OBJECT
public:
    explicit DraggableFrame(QWidget* parent = nullptr);
    static QList<DraggableFrame*> generatedFrame;
    QPoint PFrame;
    QString name;
    bool signalEmitted=false;
    QString savePath;
    int borderstatus;
    void setpath(QString imagepath);
    void setIcon(const QIcon& icon);
    void setIconSize(const QSize& size);
    int matchId;
    int getmatchID();
    void setMatchId(int id);
    int imageCounter;
    QString imagePath;
    QFrame* control;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    bool drawblue=false;
    void copy(DraggableFrame * Frame);
    QList<QWidget*> includeWidgets;
    int frameid;
    int layers;
    bool lock=true;
protected:
    void mousePressEvent(QMouseEvent* event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event) override;//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event) override;//松开鼠标事件
    void showContextMenu(const QPoint& pos);//菜单栏展示
    void wheelEvent(QWheelEvent* event);//缩放
    void paintEvent(QPaintEvent* event) override;
    void Tree();
public slots:
    void remove();//移除控件
    void changeproperty();
    void clicktree();
    void addImage();
    void changimage();
    void changbackgroundimage();
signals:
    void clicked();
private:
    bool isDragging = false;
    QPoint dragStartPosition;
    QIcon icon;
    QSize iconSize = QSize(16, 16);
};

#endif // DRAGGABLEFRAME_H
