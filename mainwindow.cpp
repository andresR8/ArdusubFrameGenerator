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
    createMotorsList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMotorsList(){


    int motors=number_motors->text().toInt();

    for(int i=0;i< motors;i++) {

        qDebug() << i ;

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 100));
        MotorItem *motorItem = new MotorItem(this,i);


        motorsList->addItem(item);
        motorsList->setItemWidget(item, motorItem);
     }
}
