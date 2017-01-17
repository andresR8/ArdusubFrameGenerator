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
    btn_generate=ui->btnGenerate;
    txt_frame_name=ui->txtFrameName;
    connect(number_motors, SIGNAL(valueChanged(int)) , this, SLOT(createMotorsList(int)));
    connect(btn_generate, SIGNAL(clicked()) , this, SLOT(generateFrame()));

    createMotorsList(number_motors->text().toInt());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMotorsList(int value){
    for(int i=0;i< value;i++) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 100));
        MotorItem *motorItem = new MotorItem(this,i);


        motorsList->addItem(item);
        motorsList->setItemWidget(item, motorItem);
     }
}

void MainWindow::generateFrame(){

    QString frameName=txt_frame_name->toPlainText().toLower();
    frameName.replace( " ", "" );
    frameName[0] = frameName[0].toUpper(); //Formatting
    QString frameNameUpper=frameName.toUpper();

    if(frameName!=NULL && !frameName.compare("")){
      qDebug()<<"FrameName " + frameName;
      //Modify Ardusub/sub.h file
      // editFile(ARDUSUB_SUB," #define MOTOR_CLASS AP_MotorsVectored90","#elif FRAME_CONFIG ==" + frameNameUpper + "_FRAME\n #define MOTOR_CLASS AP_Motors" + frameName);

      //Modify Ardusub/config.h file
        editFile(ARDUSUB_CONFIG,"# define FRAME_CONFIG_STRING \"ROV_VECTORED90_FRAME\"","#elif FRAME_CONFIG == " + frameNameUpper + "_FRAME\n# define FRAME_CONFIG_STRING \"ROV_" + frameNameUpper+ "_FRAME\"");

    }
    else
        qDebug()<<"Frame Name null";
}

void MainWindow::editFile(QString filePath, QString tag, QString add){
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QRegularExpression re(tag);
    QString dataText = file.readAll();
    file.close();
    QString replacementText(tag + "\n" + add);
    dataText.replace(re, replacementText);

    QFile newFile(filePath);
    if(newFile.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&newFile);
        out << dataText;
    }
    newFile.close();
}
