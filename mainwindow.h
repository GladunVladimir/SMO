#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stepmode.h"
#include "automode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_StepMode_clicked();

    void on_btn_AutoMode_clicked();

private:
    Ui::MainWindow *ui;
    StepMode *ui_step;
    AutoMode *ui_auto;
};
#endif // MAINWINDOW_H
