#ifndef USERLINEEDIT_H
#define USERLINEEDIT_H
#include <QApplication>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>

class UserLineEdit : public QLineEdit
{
public:
    explicit UserLineEdit(QWidget* parent = nullptr);
    void setintText(int value);
};

#endif // USERLINEEDIT_H
