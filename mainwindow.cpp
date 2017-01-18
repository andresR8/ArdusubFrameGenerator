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
    btn_about=ui->btnAbout;
    txt_frame_name=ui->txtFrameName;
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
    for(int i=0;i< value;i++) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(item->sizeHint().width(), 100));
        MotorItem *motorItem = new MotorItem(this,i);


        motorsList->addItem(item);
        motorsList->setItemWidget(item, motorItem);
     }
}

void MainWindow::generateFrame(){
    if(!checkFiles()){
        msg_error->setText("An ardusub neccesary file is missing.\nPlease check that AFG is located in the ardusub path");
        msg_error->show();
        return;
    }


    QString frameName=txt_frame_name->toPlainText().trimmed().toLower();
    frameName.replace( " ", "" ); //Formatting
    frameName[0] = frameName[0].toUpper();
    QString frameNameUpper=frameName.toUpper();
    QString frameNameLower=frameName.toLower();


    if(frameName!=NULL &&  frameName[0].unicode()!=0){
      qDebug()<<"FrameName " + frameName;
      //Modify Ardusub/sub.h file
      editFile(ARDUSUB_SUB," #define MOTOR_CLASS AP_MotorsVectored90","#elif FRAME_CONFIG ==" + frameNameUpper + "_FRAME\n #define MOTOR_CLASS AP_Motors" + frameName,false);

      //Modify Ardusub/config.h file
      editFile(ARDUSUB_CONFIG,"# define FRAME_CONFIG_STRING \"ROV_VECTORED90_FRAME\"","#elif FRAME_CONFIG == " + frameNameUpper + "_FRAME\n# define FRAME_CONFIG_STRING \"ROV_" + frameNameUpper+ "_FRAME\"",false);

      //Modify Ardusub/defines.h file
      int definesNumber=checkDefines();
      if(definesNumber==14)
        editFile(ARDUSUB_DEFINES,"#define VECTORED90_FRAME 14","#define "+ frameNameUpper + "_FRAME 15\n//ArdusubFrameGenerator#15",false);
      else
        editFile(ARDUSUB_DEFINES,"//ArdusubFrameGenerator#" + QString("%1").arg(definesNumber) , "#define "+ frameNameUpper + "_FRAME "+ QString("%1").arg(definesNumber+1) + "\n//ArdusubFrameGenerator#" + QString("%1").arg(definesNumber+1)  ,true);

       //Modify Ardusub/deploy.sh
       QString targetLine=getLineContains(ARDUSUB_DEPLOY,"TARGETS=(bluerov vectored vectored6dof simplerov");
       QString targetLineFixed=targetLine.mid(0,targetLine.indexOf("("))+"\\"+targetLine.mid(targetLine.indexOf("("),targetLine.indexOf(")")-targetLine.indexOf("("))+"\\)";
       QString ntargetLine=targetLine.mid(0,targetLine.indexOf(")")) + " " + frameNameLower + ")";
       editFile(ARDUSUB_DEPLOY,targetLineFixed,ntargetLine,true);

      //Modify libraries/AP_Motors/AP_Motors.h
       editFile(AP_MOTORS,"#include \"AP_MotorsSimpleROV.h\"" ,"#include \"AP_Motors" + frameName +"ROV.h",false);

       //Modify mk/targets.mk
       QString newTargets=getLineContains(MK_TARGETS,"FRAMES = quad tri hexa y6 octa octa-quad heli single coax");
       editFile(MK_TARGETS,newTargets,newTargets + " " +frameNameLower,true);

      //Creating file /libraries/AP_Motors/AP_MotorXXXXROV.h
       QFile file_H(AP_MF_H);
       QString newFrameH="libraries/AP_Motors/AP_Motors"+frameName+"ROV.h";
       file_H.copy(newFrameH);
       editFile(newFrameH, "/// @file	AP_MotorsSimpleROV.h" , "/// @file	AP_Motors"+ frameName +"ROV.h", true);
       editFile(newFrameH,"/// @brief	Motor control class for SimpleROV" ,"/// @brief	Motor control class for " + frameName + "ROV generated with ArdusubFrameGenerator",true);
       editFile(newFrameH, "#ifndef __AP_MOTORS_SIMPLEROV_H__\n#define __AP_MOTORS_SIMPLEROV_H__" , "#ifndef __AP_MOTORS_" + frameNameUpper + "ROV_H__\n#define __AP_MOTORS_" + frameNameUpper + "ROV_H__" ,true);
       editFile(newFrameH, "class AP_MotorsSimpleROV : public AP_Motors6DOF {","class AP_Motors" + frameName + "ROV : public AP_Motors6DOF {",true);
       editFile(newFrameH, "/// @class      AP_MotorsSimpleROV", "/// @class      AP_Motors"+frameName+"ROV",true);
       editFile(newFrameH, "    AP_MotorsSimpleROV\\(uint16_t loop_rate, uint16_t speed_hz = AP_MOTORS_SPEED_DEFAULT\\) :", "    AP_Motors" + frameName +"ROV(uint16_t loop_rate, uint16_t speed_hz = AP_MOTORS_SPEED_DEFAULT) :",true);
       editFile(newFrameH, "#endif  // AP_MotorsSimpleROV", "#endif  // AP_Motors"+frameName+"ROV",true);


       // Creating file libraries/AP_MotorsXXXXROV.cpp
      QString newFrameCPP="libraries/AP_Motors/AP_Motors"+frameName+"ROV.cpp";
      cleanCPPfile(newFrameCPP);
      editFile(newFrameCPP," \\*       AP_MotorsSimple ROV.cpp"," *       AP_Motors"+frameName+" ROV.cpp File created with ArdusubFrameGenerator",true);
      editFile(newFrameCPP,"#include \"AP_MotorsSimpleROV.h\"","#include \"AP_Motors"+frameName+"ROV.h\"",true);
      editFile(newFrameCPP,"// setup_motors - configures the motors for the BlueROV","// setup_motors - configures the motors for the "+ frameName+ " ROV",true);
      editFile(newFrameCPP,"void AP_MotorsSimpleROV::setup_motors\\(\\)","void AP_Motors"+frameName+"ROV::setup_motors()",true);
      editFile(newFrameCPP,"	// hard coded config for Simple ROV","	// hard coded config for "+frameName+" ROV",true);

      QString motorDefs=NULL;

      for  (int i=0; i < motorsList->count();i++) {

          QListWidgetItem* item = motorsList->item(i);
          MotorItem* m=(MotorItem*)motorsList->itemWidget(item)  ;
          motorDefs.append(m->getMotorDefinition());
          motorDefs.append("\n");
      }

      motorDefs[motorDefs.size()-3]=';';

      for  (int i=0; i < motorsList->count();i++) {
          QListWidgetItem* item = motorsList->item(i);
          MotorItem* m=(MotorItem*)motorsList->itemWidget(item)  ;
          motorDefs.append(m->getMotorAdd());
          motorDefs.append("\n");
      }

      editFile(newFrameCPP,"		// Right forward thruster",motorDefs,true);


      //Done
        msg_done->setText("The new frame has been created:\n\nTo compile please use:\n      make px4-v2-"+ frameNameLower + "\n\nTo compile and upload use:\n     make px4-v2-"+ frameNameLower + "-upload");
        msg_done->show();

    }
    else{
        msg_error->setText("Please fill the frame name field");
        msg_error->show();
    }
}

int MainWindow::checkDefines(){

    QString line=getLineContains(ARDUSUB_DEFINES,"//ArdusubFrameGenerator#");

    if(line!=NULL)
            return line.mid(line.indexOf("#")+1,line.size()-1).toInt();

    return 14;

}

void MainWindow::cleanCPPfile(QString cppFile){
    QFile file(AP_MF_CPP);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString dataOut=NULL;
    bool initClean=false;
    bool stopClean=false;

    while (!in.atEnd()){
        QString line=in.readLine();

         if(!initClean){
            initClean=line.contains("		MOT_1_ROLL_FACTOR = 0,");
         }
         else if(!stopClean){
            stopClean=line.contains("	add_motor_raw_6dof(AP_MOTORS_MOT_5, MOT_5_ROLL_FACTOR, MOT_5_PITCH_FACTOR, MOT_5_YAW_FACTOR, MOT_5_THROTTLE_FACTOR, MOT_5_FORWARD_FACTOR, MOT_5_STRAFE_FACTOR,5);");
            line=in.readLine();
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


QString MainWindow::getLineContains(QString filePath, QString contains){
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString dataOut=NULL;

    while (!in.atEnd()){
        QString line=in.readLine();
         if(line.contains(contains)){
           dataOut=line;
           break;
         }
    }
    file.close();

    return dataOut;
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


bool MainWindow::checkFiles(){
    QFile f(ARDUSUB_CONFIG);
    if(!f.exists()) return false;

    QFile f1(ARDUSUB_DEFINES);
    if(!f1.exists()) return false;

    QFile f2(ARDUSUB_DEPLOY);
    if(!f2.exists()) return false;

    QFile f3(ARDUSUB_SUB);
    if(!f3.exists()) return false;

   QFile f4(AP_MOTORS);
    if(!f4.exists()) return false;

    QFile f5(MK_TARGETS);
    if(!f5.exists()) return false;

    QFile f6(AP_MF_H);
    if(!f6.exists()) return false;

    QFile f7(AP_MF_CPP);
    if(!f7.exists()) return false;

    return true;
}

void MainWindow::aboutMsg(){
    QMessageBox *about=new QMessageBox();
    about->setText("This program was created by Andrés Rengifo as a contribution to Ardusub Project more info at:\n\nhttps://github.com/andresR8/ardusubFrameGenerator");
    about->setIcon(QMessageBox::Information);
    about->setStyleSheet("border: 20px; border-radius:15px;\nbackground-color: rgb(39, 43, 44);font-size: 10;\ncolor: rgb(255, 255, 255);\n");
    about->show();
}
