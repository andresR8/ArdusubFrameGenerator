#ifndef MOTORITEM_H
#define MOTORITEM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLine>
#include <QEvent>
#include <QDebug>

#define LW_STYLE1 "background-color: rgb(192,192,192) ;"
#define LW_STYLE2 "border-color: black;"

class MotorItem : public QWidget
{
    Q_OBJECT
public:
    explicit MotorItem(QWidget *parent = 0, int motorNum=0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);


private:
    QVBoxLayout *MainLayout;
    QHBoxLayout *HLayout1, *HLayout2;
    QLabel *lblMotorChannel,*lblRollFactor,*lblPitchFactor, *lblYawFactor, *lblThrottleFactor, *lblFordwardFactor, *lblLateralFactor;
    QDoubleSpinBox  *spRollFactor, *spPitchFactor,  *spYawFactor,  *spThrottleFactor,  *spFordwardFactor,  *spLateralFactor;
    QSpinBox *spMotorChannel;
    QWidget *spacer;

public:
    QString getMotorAdd();
};

#endif // MOTORITEM_H
