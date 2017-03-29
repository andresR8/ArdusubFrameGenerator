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
    motorsList->setDragEnabled(false);
    number_motors=ui->spin_motors;
    btn_generate=ui->btnGenerate;
    btn_about=ui->btnAbout;    
    connect(number_motors, SIGNAL(valueChanged(int)) , this, SLOT(createMotorsList(int)));
    connect(btn_generate, SIGNAL(clicked()) , this, SLOT(generateFrame()));
    connect(btn_about, SIGNAL(clicked()) , this, SLOT(aboutMsg()));

    msg_error=new QMessageBox();
    msg_error->setIcon(QMessageBox::Critical);
    msg_error->setStyleSheet("border: 20px; border-radius:15px;\nbackground-color: rgb(39, 43, 44);font-size: 10;\ncolor: rgb(255, 255, 255);\n");

    msg_done=new QMessageBox();
    msg_done->setIcon(QMessageBox::Information);
    msg_done->setStyleSheet("border: 20px; border-radius:15px;\nbackground-color: rgb(39, 43, 44);font-size: 10;\ncolor: rgb(255, 255, 255);\n");


    createMotorsList(number_motors->text().toInt());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMotorsList(int value){
    motorsList->clear();
    for(int i=0;i< value;i++) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 100));
        MotorItem *motorItem = new MotorItem(this,i);


        motorsList->addItem(item);
        motorsList->setItemWidget(item, motorItem);
     }
}

void MainWindow::generateFrame(){
    QFile f(AP_FRAME_TARGET);
    if(!f.exists()){
        msg_error->setText("An ardusub neccesary file is missing.\nPlease check that AFG is located in the ardusub path");
        msg_error->show();
        return;
    }

       // Modifiying file libraries/AP_Motors6DOF.cpp
      QString motorDefs=NULL;

      for  (int i=0; i < motorsList->count();i++) {
          QListWidgetItem* item = motorsList->item(i);
          MotorItem* m=(MotorItem*)motorsList->itemWidget(item)  ;
          motorDefs.append(m->getMotorAdd());         

          if(i!=motorsList->count()-1)
              motorDefs.append("\n");
      }


      if(getLineContains(AP_FRAME_TARGET,"	    	break;// ArdusubFrameGenerator"))
          cleanCPPfile(AP_FRAME_TARGET);
      else
            motorDefs.append("\n	    	break;// ArdusubFrameGenerator");

      editFile(AP_FRAME_TARGET,"	    case AS_MOTORS_CUSTOM_FRAME:",motorDefs,false);


      //Done
      msg_done->setText("The new frame has been created:\n\nTo compile please use:\n      make px4-v2\n\nTo compile and upload use:\n     make px4-v2-upload");
      msg_done->show();



}


void MainWindow::cleanCPPfile(QString cppFile){
    QFile file(cppFile);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString dataOut=NULL;
    bool initClean=false;
    bool stopClean=false;

    while (!in.atEnd()){
        QString line=in.readLine();
         if(!initClean){
            initClean=line.contains("	    case AS_MOTORS_CUSTOM_FRAME:");
         }
         else if(!stopClean){
            stopClean=line.contains("	    	break;// ArdusubFrameGenerator");
            if(stopClean)
                dataOut=dataOut+"	    case AS_MOTORS_CUSTOM_FRAME:\n";
         }


         if(!initClean || stopClean)
             dataOut=dataOut+line+"\n";
    }
    file.close();

    QFile newFile(cppFile);

    if(newFile.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&newFile);
        newFile.write(dataOut.toUtf8());
    }
    newFile.close();
}


bool MainWindow::getLineContains(QString filePath, QString contains){
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString dataOut=NULL;

    while (!in.atEnd()){
        QString line=in.readLine();
         if(line.contains(contains)){
           return true;
         }
    }
    file.close();

    return false;
}

void MainWindow::editFile(QString filePath, QString tag, QString add, bool overwrite){
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QRegularExpression re(tag);
    QString dataText = file.readAll();
    file.close();


    QString replacementText(tag + "\n" + add);
    if(overwrite)
    replacementText=add;

    dataText.replace(re,replacementText);

    QFile newFile(filePath);

    if(newFile.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&newFile);
        newFile.write(dataText.toUtf8());
    }
    newFile.close();
}


void MainWindow::aboutMsg(){
    QMessageBox *about=new QMessageBox();
    about->setText("This program was created by Andrés Rengifo as a contribution to Ardusub Project more info at:\n\nhttps://github.com/andresR8/ardusubFrameGenerator");
    about->setIcon(QMessageBox::Information);
    about->setStyleSheet("border: 20px; border-radius:15px;\nbackground-color: rgb(39, 43, 44);font-size: 10;\ncolor: rgb(255, 255, 255);\n");
    about->show();
}
