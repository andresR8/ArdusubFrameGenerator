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


#define ARDUSUB_SUB     "ArduSub/Sub.h"
#define ARDUSUB_CONFIG  "ArduSub/config.h"
#define ARDUSUB_DEFINES "ArduSub/defines.h"
#define ARDUSUB_DEPLOY  "ArduSub/deploy.sh"
#define AP_MOTORS       "libraries/AP_Motors/AP_Motors.h"
#define MK_TARGETS      "mk/targets.mk"
#define LIBRARIES       "libraries/"




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


private:
    Ui::MainWindow *ui;
    QListWidget *motorsList;
    QSpinBox *number_motors;
    QPushButton *btn_generate;
    QTextEdit *txt_frame_name;
    //Private functions
    void editFile(QString filePath, QString tag, QString add, bool overwrite);
    int checkDefines();
    void createDeploy(QString frameNameLower);
    QString getLineContains(QString file, QString contains);
};

#endif // MAINWINDOW_H

