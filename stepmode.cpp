#include "stepmode.h"
#include "ui_stepmode.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include "request.h"
#include "selectionmanager.h"
#include "device.h"
#include "source.h"
#include "stagingmanager.h"
#include <QMessageBox>
#include <deque>

StepMode::StepMode(QWidget *parent, unsigned int count_of_sources,
                   unsigned int count_of_divece,
                   unsigned int count_of_requests,
                   unsigned int buffer_size,
                   double lambda_source,
                   double alpha_device,
                   double beta_device)
    : QMainWindow(parent)
    , ui(new Ui::StepMode)
    , count_of_sources(count_of_sources)
    , count_of_divece(count_of_divece)
    , count_of_requests(count_of_requests)
    , buffer_size(buffer_size)
    , current_number_request()
    , current_step()
    , current_time()
    , lambda_source(lambda_source)
    , alpha_device(alpha_device)
    , beta_device(beta_device)
    , lbl_Title_Source()
    , lbl_Content_Source()
    , lbl_Title_Buffer()
    , lbl_Content_Buffer()
    , lbl_Title_Device()
    , lbl_Content_Device()
    , source()
    , stagingmanager()
    , selectionmanager()
    , device()
    , buffer()
{

    this->setCount_of_sources(count_of_sources);
    this->setCount_of_divece(count_of_divece);
    this->setCount_of_requests(count_of_requests);
    this->setBuffer_size(buffer_size);
    this->setLambda_source(lambda_source);
    this->setAlpha_device(alpha_device);
    this->setBeta_device(beta_device);
    ui->setupUi(this);
    setCentralWidget(this->ui->splitter);
    this->count_of_sources = count_of_sources;
    this->count_of_divece = count_of_divece;
    this->count_of_requests = count_of_requests;
    this->buffer_size = buffer_size;


    for(unsigned int i = 0; i < this->getCount_of_sources(); i++)
    {

        int q = static_cast<int>(i);
        this->lbl_Title_Source << new QLabel;
        this->lbl_Content_Source << new QLabel;
        QString cur_number = QString::number(i + 1);
        this->lbl_Title_Source[q]->setText("Источник # " + cur_number);
        this->lbl_Content_Source[q]->setStyleSheet(this->style + "border-color #000000;" + "background-color: #ffffff;" + "color: #000000;");
        this->ui->VL_Source->addWidget(this->lbl_Title_Source[q]);
        this->ui->VL_Source->addWidget(this->lbl_Content_Source[q]);

    }


    for(unsigned int i = 0; i < this->buffer_size; i++)
    {

        int q = static_cast<int>(i);
        this->lbl_Title_Buffer << new QLabel;
        this->lbl_Content_Buffer << new QLabel;
        QString cur_number = QString::number(i + 1);
        this->lbl_Title_Buffer[q]->setText("Буфер # " + cur_number);
        this->lbl_Content_Buffer[q]->setStyleSheet(this->style + "border-color #000000;" + "background-color: #ffffff;" + "color: #000000;");
        this->ui->VL_Buffer->addWidget(this->lbl_Title_Buffer[q]);
        this->ui->VL_Buffer->addWidget(this->lbl_Content_Buffer[q]);

    }


    for(unsigned int i = 0; i < this->count_of_divece; i++)
    {

        int q = static_cast<int>(i);
        this->lbl_Title_Device << new QLabel;
        this->lbl_Content_Device << new QLabel;
        QString cur_number = QString::number(i + 1);
        this->lbl_Title_Device[q]->setText("Прибор # " + cur_number);
        this->lbl_Content_Device[q]->setStyleSheet(this->style + "border-color #000000;" + "background-color: #ffffff;" + "color: #000000;");
        this->ui->VL_Device->addWidget(this->lbl_Title_Device[q]);
        this->ui->VL_Device->addWidget(this->lbl_Content_Device[q]);

    }


    this->stagingmanager.setPointer_on_add(0);
    this->stagingmanager.setBuffer_size(this->getBuffer_size());
    this->selectionmanager.setBuffer_size(this->getBuffer_size());
    this->selectionmanager.setCount_of_device(this->getCount_of_divece());


    this->source.data_init(getCount_of_sources(), getLambda_source());
    for (unsigned int i = 0; i < getCount_of_divece(); i++)
    {

        this->device[i].setAlpha(this->getAlpha_device());
        this->device[i].setBeta(this->getBeta_device());
        this->device[i].operating_ratio = 0.0;

    }

    //генерация необходимого для моделирования количества заявок
    for (unsigned int i = 0; i < this->getCount_of_requests(); i++)
    {

        this->source.create_request(&current_request.number_of_source, &current_request.time, this->current_time);
        this->setCurrent_time(current_request.getTime());
        time_of_moments.push_back(current_request);

    }
}

StepMode::~StepMode()
{
    delete ui;
}

void StepMode::on_btnBack_clicked()
{
    this->hide();
    parentWidget()->show();

}

void StepMode::on_btnPlusStep_clicked()
{
    this->current_step++;
    this->ui->txtInfo->append("Шаг " + QString::number(this->getCurrent_step()));


    this->lbl_Content_Buffer[static_cast<int>(this->stagingmanager.getPointer_on_add())]->
            setStyleSheet(this->style + "border-color #000000;" + "background-color: #ffffff;" + "color: #000000;");
    this->lbl_Content_Buffer[static_cast<int>(this->selectionmanager.getPointer_on_get())]->
            setStyleSheet(this->style + "border-color #000000;" + "background-color: #ffffff;" + "color: #000000;");
    this->lbl_Content_Device[static_cast<int>(this->selectionmanager.getPointer_on_push())]->
            setStyleSheet(this->style + "border-color #000000;" + "background-color: #ffffff;" + "color: #000000;");


    this->ui->statusbar->showMessage("Время: " + QString::number(time_of_moments[0].getTime()));

    //проверка на наличие события
    if(not time_of_moments.empty())
    {

        //проверка текущего события(Заявка ли это?)
        if(time_of_moments[0].getStatus() == 0)
        {

            //добавление заявки в буффер
            this->source.count_of_request[time_of_moments[0].number_of_source]++;
            time_of_moments[0].number_of_request = static_cast<unsigned int>(source.count_of_request[time_of_moments[0].number_of_source]);
            this->stagingmanager.add_to_buffer(time_of_moments[0], this->buffer, &this->source, time_of_moments[0].getTime());

            //вывод на экран текущего состояния
            unsigned int poa = this->stagingmanager.getPointer(this->stagingmanager.getPointer_on_add(), this->getBuffer_size());
            lbl_Content_Source[static_cast<int>(time_of_moments[0].getNumber_of_source())]->
                                    setText(QString::number(time_of_moments[0].getNumber_of_source() + 1) + "."
                                            + QString::number(this->source.count_of_request[time_of_moments[0].getNumber_of_source()]) +
                                            "  время:  " + QString::number(time_of_moments[0].getTime()));
            lbl_Content_Buffer[static_cast<int>(poa)]->
                    setText(QString::number(time_of_moments[0].getNumber_of_source() + 1) + "."
                            + QString::number(source.count_of_request[time_of_moments[0].getNumber_of_source()])
                    + "  время:  " + QString::number(time_of_moments[0].getTime()));
            this->ui->txtInfo->append("С источника " + QString::number(time_of_moments[0].getNumber_of_source() + 1) + " пришла заявка с номером " + QString::number(time_of_moments[0].getNumber_of_request()) + " и была добавлена в прибор на " +
                                      QString::number(poa + 1) + " место. \n");



            // поиск свободного места в приборе
            bool free_place_in_device = false;
            for(unsigned int i = 0; i < this->getCount_of_divece(); i++)
            {
                if (device[i].is_empty(time_of_moments[0].getTime()))
                {
                    free_place_in_device = true;
                    this->selectionmanager.setPointer_on_push(i);
                    break;
                }
            }
            if(free_place_in_device)
            {

                //измение status у текущего события(добавление в заявки в буффер -> добавление заявки на прибор)
                time_of_moments[0].setStatus(1);

            }else {

                //удаление текущего события
                time_of_moments.pop_front();
                this->ui->txtInfo->append("Заявке отказано\n");
            }

        }else{

            //вычисление загруженности приборов
            for (unsigned int i = 0; i < this->getCount_of_divece(); i++)
            {
                if((device[i].sum_time_working / time_of_moments[0].getTime()) * 100.0 < 100.0)
                {

                    device[i].operating_ratio = (device[i].sum_time_working / time_of_moments[0].getTime()) * 100.0;

                }else{

                    device[i].operating_ratio = 100.0;

                }
            }

            //вычисление количества свободных мест в буффере
            unsigned int sum_free_place_in_buffer = 0;
            for(unsigned int i = 0; i < this->getBuffer_size(); i++)
            {
                if(this->buffer[i].getIs_empty())
                {
                   sum_free_place_in_buffer++;
                }
            }
            if(sum_free_place_in_buffer == this->getBuffer_size())
            {

                //удаление текущего события
                time_of_moments.pop_front();



            }else{

                //добавление заявки из буффера на прибор
                //сортировка событий по времени наступления
                this->selectionmanager.add_to_device(this->buffer, this->device, time_of_moments[0].getTime(), &this->source);
  //            unsigned int pop = this->selectionmanager.getPointer(this->selectionmanager.getPointer_on_push(), this->getCount_of_divece());
                time_of_moments[0].setTime(device[this->selectionmanager.getPointer_on_push()].getRelease_time());

                for (unsigned int i = 0; i < time_of_moments.size(); i++)
                {
                    for (unsigned int j = i; j < time_of_moments.size(); j++) {
                        if(time_of_moments[i].getTime() > time_of_moments[j].getTime())
                        {
                            auto rec = time_of_moments[i];
                            time_of_moments[i] = time_of_moments[j];
                            time_of_moments[j] = rec;
                        }
                    }
                }


                //вывод на экран текущего состояния
//                unsigned int pog = this->selectionmanager.getPointer(this->selectionmanager.getPointer_on_get(), this->getBuffer_size());
                lbl_Content_Device[static_cast<int>(this->selectionmanager.getPointer_on_push())]->
                        setText(QString::number(this->device[this->selectionmanager.getPointer_on_push()].getNumber_of_source() + 1) + "." +
                        QString::number(this->device[this->selectionmanager.getPointer_on_push()].getNumber_of_request())
                                + "  время:  " + QString::number(this->device[this->selectionmanager.getPointer_on_push()].getRelease_time()));
                lbl_Content_Buffer[static_cast<int>(this->selectionmanager.getPointer_on_get())]->clear();
                this->ui->txtInfo->append("Заявка с " +
                                          QString::number(this->selectionmanager.getPointer_on_get() + 1) + " места в буфере"
                                          "  была добавлена на " +
                                          QString::number(this->selectionmanager.getPointer_on_push() + 1) + " место в приборе. \n");

            }
        }






//        qDebug() << this->current_number_request;
//        qDebug() << time_of_moments.size() << " - step: " << count;

//        qDebug() << "_______________________________________";
//        for (unsigned int i = 0; i < time_of_moments.size(); i++)
//        {
//            qDebug() << time_of_moments[i].getTime();
//        }

        if (this->stagingmanager.getPointer_on_add() == this->selectionmanager.getPointer_on_get())
        {
            this->lbl_Content_Buffer[static_cast<int>(this->stagingmanager.getPointer_on_add())]->
                    setStyleSheet(this->style + "border-color: #ff0000;" + "background-color: #00ff00;" + "color: #000000;");
        }else
        {

            this->lbl_Content_Buffer[static_cast<int>(this->stagingmanager.getPointer_on_add())]->
                    setStyleSheet(this->style + "background-color: #00ff00;" + "border-color: #000000;" + "color: #000000;");
            this->lbl_Content_Buffer[static_cast<int>(this->selectionmanager.getPointer_on_get())]->
                    setStyleSheet(this->style + "border-color: #ff0000;" + "background-color: #ffffff;" + "color: #000000;");
        }
        this->lbl_Content_Device[static_cast<int>(this->selectionmanager.getPointer_on_push())]->
                setStyleSheet(this->style + "background-color: #0000ff;" + "border-color #000000;" + "color: #000000;");


    }else
    {
        double last_device_time = 0.0;
        for(unsigned int i = 0; i < this->getCount_of_divece(); i++)
        {
            if (this->device[i].getRelease_time() > last_device_time)
            {
                last_device_time = this->device[i].getRelease_time();
            }
        }
        this->current_time += last_device_time;
        this->ui->txtInfo->append("Моделирование было завершено со временем: " + QString::number(this->getCurrent_time()));
        QMessageBox::StandardButton AYSure = QMessageBox::question(this, "Конец моделирования!", "Вернуться в главное окно!");
        if (AYSure == QMessageBox::Yes)
        {
            this->hide();
            parentWidget()->show();
        }
    }


}

double StepMode::getBeta_device() const
{
    return beta_device;
}

void StepMode::setBeta_device(double value)
{
    beta_device = value;
}

double StepMode::getAlpha_device() const
{
    return alpha_device;
}

void StepMode::setAlpha_device(double value)
{
    alpha_device = value;
}

double StepMode::getLambda_source() const
{
    return lambda_source;
}

void StepMode::setLambda_source(double value)
{
    lambda_source = value;
}

double StepMode::getCurrent_time() const
{
    return current_time;
}

void StepMode::setCurrent_time(double value)
{
    current_time = value;
}

unsigned int StepMode::getCurrent_step() const
{
    return current_step;
}

void StepMode::setCurrent_step(unsigned int value)
{
    current_step = value;
}

unsigned int StepMode::getCurrent_number_request() const
{
    return current_number_request;
}

void StepMode::setCurrent_number_request(unsigned int value)
{
    current_number_request = value;
}

unsigned int StepMode::getBuffer_size() const
{
    return buffer_size;
}

void StepMode::setBuffer_size(unsigned int value)
{
    buffer_size = value;
}

unsigned int StepMode::getCount_of_requests() const
{
    return count_of_requests;
}

void StepMode::setCount_of_requests(unsigned int value)
{
    count_of_requests = value;
}

unsigned int StepMode::getCount_of_divece() const
{
    return count_of_divece;
}

void StepMode::setCount_of_divece(unsigned int value)
{
    count_of_divece = value;
}

unsigned int StepMode::getCount_of_sources() const
{
    return count_of_sources;
}

void StepMode::setCount_of_sources(unsigned int value)
{
    count_of_sources = value;
}

