#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "source.h"
#include "stagingmanager.h"
#include <QDebug>
#include "request.h"
#include "selectionmanager.h"
#include "device.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralwidget->setStyleSheet("background-color: rgb(255, 255, 255);");
    setWindowTitle("Курсовая работа (СМО)");
    QPixmap icon = QPixmap(1,1);
    icon.fill(QColor(0,0,0,0));
    setWindowIcon(QIcon(icon));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_StepMode_clicked()
{
    QMessageBox::StandardButton AYSure = QMessageBox::question(this, "Are you sure?",
                            "Количество источников = " + ui->spin_COSources->text() + '\n'
                          + "Количество приборов = " + ui->spin_CODevice->text() + '\n'
                          + "Количество заявок = " + ui->spin_CORequests->text() + '\n'
                          + "Размер буфера = " + ui->spin_BSize->text() + '\n'
                          + "Интенсивность источников = " + QString::number(ui->spin_lam_source->value()) + '\n'
                          + "Альфа приборов = " + QString::number(ui->spin_alpha_device->value()) + '\n'
                          + "Бета приборов = " + QString::number(ui->spin_beta_device->value()) + '\n');
    if (AYSure == QMessageBox::Yes)
    {
        this->hide();
        unsigned int count_of_sources = ui->spin_COSources->text().toUInt();
        unsigned int count_of_device = ui->spin_CODevice->text().toUInt();
        unsigned int count_of_requests = ui->spin_CORequests->text().toUInt();
        unsigned int buffer_size = ui->spin_BSize->text().toUInt();
        double lambda_source = this->ui->spin_lam_source->value();
        double alpha_device = this->ui->spin_alpha_device->value();
        double beta_device = this->ui->spin_beta_device->value();
        ui_step = new StepMode(this,
                               count_of_sources,
                               count_of_device,
                               count_of_requests,
                               buffer_size,
                               lambda_source,
                               alpha_device,
                               beta_device);
        ui_step->showFullScreen();

    }
}

void MainWindow::on_btn_AutoMode_clicked()
{
    QMessageBox::StandardButton AYSure = QMessageBox::question(this, "Are you sure?",
                               "Количество источников = " + ui->spin_COSources->text() + '\n'
                             + "Количество приборов = " + ui->spin_CODevice->text() + '\n'
                             + "Количество заявок = " + ui->spin_CORequests->text() + '\n'
                             + "Размер буфера = " + ui->spin_BSize->text() + '\n'
                             + "Интенсивность источников = " + QString::number(ui->spin_lam_source->value()) + '\n'
                             + "Альфа приборов = " + QString::number(ui->spin_alpha_device->value()) + '\n'
                             + "Бета приборов = " + QString::number(ui->spin_beta_device->value()) + '\n');
    if (AYSure == QMessageBox::Yes)
    {
        this->hide();
        unsigned int count_of_sources = ui->spin_COSources->text().toUInt();
        unsigned int count_of_device = ui->spin_CODevice->text().toUInt();
        unsigned int count_of_requests = ui->spin_CORequests->text().toUInt();
        unsigned int buffer_size = ui->spin_BSize->text().toUInt();
        double lambda_source = this->ui->spin_lam_source->value();
        double alpha_device = this->ui->spin_alpha_device->value();
        double beta_device = this->ui->spin_beta_device->value();
        ui_auto = new AutoMode(this,
                               count_of_sources,
                               count_of_device,
                               count_of_requests,
                               buffer_size,
                               lambda_source,
                               alpha_device,
                               beta_device);
        ui_auto->show();

    }
}

