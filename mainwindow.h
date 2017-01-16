#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QSpinBox>
#include <QDebug>
#include "motoritem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QListWidget *motorsList;
    QSpinBox *number_motors;
    //Private functions
    void createMotorsList();
};

#endif // MAINWINDOW_H
