#ifndef USERDIAL_H
#define USERDIAL_H
#include "QDial"

class UserDial : public QDial
{
    Q_OBJECT
public:
    explicit UserDial(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void drawTunerKnob(QPainter *painter);
};

#endif // USERDIAL_H
