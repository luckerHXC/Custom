#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(""); // 设置窗口标题为空字符串
    w.show();

    return a.exec();
}
