#include "motoritem.h"

MotorItem::MotorItem(QWidget *parent, int motorNum) :
    QWidget(parent)
{
      MainLayout=new QVBoxLayout();
      HLayout1=new QHBoxLayout();
      HLayout2=new QHBoxLayout();


      spacer= new QWidget;
      spacer ->setFixedHeight(2);
      spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      spacer->setStyleSheet(QString("background-color: black;"));


      lblMotorChannel=new QLabel("Motor\nChannel");
      lblMotorChannel->setAlignment(Qt::AlignCenter);
      lblPitchFactor=new QLabel("Pitch\nFactor");
      lblPitchFactor->setAlignment(Qt::AlignCenter);
      lblRollFactor=new QLabel("Roll\nFactor");
      lblRollFactor->setAlignment(Qt::AlignCenter);
      lblYawFactor=new QLabel("Yaw\nFactor");
      lblYawFactor->setAlignment(Qt::AlignCenter);
      lblThrottleFactor= new QLabel("Thottle\nFactor");
      lblThrottleFactor->setAlignment(Qt::AlignCenter);
      lblFordwardFactor=new QLabel("Fordward\nFactor");
      lblFordwardFactor->setAlignment(Qt::AlignCenter);
      lblStrafeFactor=new QLabel("Strafe\nFactor");
      lblStrafeFactor->setAlignment(Qt::AlignCenter);


       spMotorChannel=new QSpinBox();
       spMotorChannel->setAlignment(Qt::AlignCenter);
       spMotorChannel->setMaximum(8);
       spMotorChannel->setMinimum(1);
       spMotorChannel->setValue(motorNum+1);
       spMotorChannel->installEventFilter(this);


       spPitchFactor=new QDoubleSpinBox();
       spPitchFactor->setAlignment(Qt::AlignCenter);
       spPitchFactor->setMaximum(1);
       spPitchFactor->setMinimum(-1);
       spPitchFactor->installEventFilter(this);

       spRollFactor=new QDoubleSpinBox();
       spRollFactor->setMaximum(1);
       spRollFactor->setMinimum(-1);
       spRollFactor->setAlignment(Qt::AlignCenter);
       spRollFactor->installEventFilter(this);

       spYawFactor=new QDoubleSpinBox();
       spYawFactor->setMaximum(1);
       spYawFactor->setMinimum(-1);
       spYawFactor->setAlignment(Qt::AlignCenter);
       spYawFactor->installEventFilter(this);

       spThrottleFactor= new QDoubleSpinBox();
       spThrottleFactor->setMaximum(1);
       spThrottleFactor->setMinimum(-1);
       spThrottleFactor->setAlignment(Qt::AlignCenter);
       spThrottleFactor->installEventFilter(this);

       spFordwardFactor=new QDoubleSpinBox();
       spFordwardFactor->setMaximum(1);
       spFordwardFactor->setMinimum(-1);
       spFordwardFactor->setAlignment(Qt::AlignCenter);
       spFordwardFactor->installEventFilter(this);

       spStrafeFactor=new QDoubleSpinBox();
       spStrafeFactor->setMaximum(1);
       spStrafeFactor->setMinimum(-1);
       spStrafeFactor->setAlignment(Qt::AlignCenter);
       spStrafeFactor->installEventFilter(this);

       HLayout1->addWidget(lblMotorChannel);
       HLayout1->addWidget(lblPitchFactor);
       HLayout1->addWidget(lblRollFactor);
       HLayout1->addWidget(lblYawFactor);
       HLayout1->addWidget(lblThrottleFactor);
       HLayout1->addWidget(lblFordwardFactor);
       HLayout1->addWidget(lblStrafeFactor);

       HLayout2->addWidget(spMotorChannel);
       HLayout2->addWidget(spPitchFactor);
       HLayout2->addWidget(spRollFactor);
       HLayout2->addWidget(spYawFactor);
       HLayout2->addWidget(spThrottleFactor);
       HLayout2->addWidget(spFordwardFactor);
       HLayout2->addWidget(spStrafeFactor);




       MainLayout->addWidget(spacer);
       MainLayout->addLayout(HLayout1);
       MainLayout->addLayout(HLayout2);
       MainLayout->addWidget(spacer);



       this->setLayout(MainLayout);

}

bool MotorItem::eventFilter( QObject * o, QEvent * e ) {
   if ( e->type() == QEvent::Wheel &&
        qobject_cast<QAbstractSpinBox*>( o ) )
   {
       e->ignore();
       return true;
   }
   return QWidget::eventFilter( o, e );
}
