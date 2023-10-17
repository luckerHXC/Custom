#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "generate.h"
#include <QMainWindow>
#include "controlproperty.h"
#include "connectsl.h"
#include "generate.h"
#include "QtWebSockets/QWebSocket"
#include <QMap>
#include <QPointer>
#include "widgethelper.h"
#include "clickablelabel.h"
#include "draggablevolumebar.h"
#include "uservolumebar.h"
class connectSL;
namespace Ui {
class MainWindow;
}
class controlproperty;
class generate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    controlproperty * cp;
    generate * gen;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void matchSL();
    QPoint m_rightPoint;
    QMap<QPointer<QWidget>, QString> styleMap;
    QList<QWidget*> m_selectedWidgets;
    int getWidgetDepth(QWidget* widget);
    int layers=1;
    int downlayers=0;


public slots:    
    void onScrollValueChanged(int value);//滚屏函数
    void onTabChanged(int index);
    void sendData();
    void onConnected();
    void onDisconnected();
    void restorecontrol();
    void actionSelectedWidget(QAction *action);
private:
    DraggableSlider* connectedSlider = nullptr;
    DraggableLineEdit* connectedLineEdit = nullptr;
    DraggableVolumeBar* connectedVolumeBar = nullptr;
    int nextMatchId = 1;
    bool m_isDrawing;
    QPoint m_startPoint;
    QPoint m_endPoint;
    QRect m_selectionRect;
    QPoint startPoint;
    QPoint endPoint;
    QPushButton *m_sendButton;
    QWebSocket *m_webSocket;
    bool choicewidget=false;
};

#endif // MAINWINDOW_H
