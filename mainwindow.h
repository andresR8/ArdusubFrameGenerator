#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QSpinBox>
#include <QDebug>
#include "motoritem.h"
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>

#define AP_FRAME_TARGET      "libraries/AP_Motors/AP_Motors6DOF.cpp"
#define AP_CASE_LINE         "    case SUB_FRAME_CUSTOM:"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void createMotorsList(int value);
    void generateFrame();
    void aboutMsg();


private:
    Ui::MainWindow *ui;
    QListWidget *motorsList;
    QSpinBox *number_motors;
    QPushButton *btn_generate, *btn_about;
    QTextEdit *txt_frame_name;
    QMessageBox *msg_done, *msg_error;
    //Private functions
    void editFile(QString filePath, QString tag, QString add, bool overwrite);
    bool getLineContains(QString filePath, QString contains);
    void cleanCPPfile(QString cppFile);


};

#endif // MAINWINDOW_H

