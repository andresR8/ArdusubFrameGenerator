#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyle>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    );
    ui->setupUi(this);
    motorsList=ui->motors_list;
    number_motors=ui->spin_motors;
    connect(number_motors, SIGNAL(valueChanged(int)) , this, SLOT(createMotorsList(int)));
    createMotorsList(number_motors->text().toInt());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMotorsList(int value){

    for(int i=0;i< value;i++) {

        qDebug() << i ;

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 100));
        MotorItem *motorItem = new MotorItem(this,i);


        motorsList->addItem(item);
        motorsList->setItemWidget(item, motorItem);
     }
}
