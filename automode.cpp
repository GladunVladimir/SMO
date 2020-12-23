#include "automode.h"
#include "ui_automode.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include <deque>


AutoMode::AutoMode(QWidget *parent, unsigned int count_of_sources,
                   unsigned int count_of_divece,
                   unsigned int count_of_requests,
                   unsigned int buffer_size,
                   double lambda_source,
                   double alpha_device,
                   double beta_device) :
    QMainWindow(parent),
    ui(new Ui::AutoMode),
    count_of_sources(count_of_sources),
    count_of_divece(count_of_divece),
    count_of_requests(count_of_requests),
    buffer_size(buffer_size),
    current_step(),
    current_number_request(),
    current_time(),
    lambda_source(lambda_source),
    alpha_device(alpha_device),
    beta_device(beta_device),
    source(),
    stagingmanager(),
    selectionmanager(),
    device(),
    buffer()
{

    // установка начальных значений
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




    Request current_request;
    deque<Request> time_of_moments;

    //генерация необходимого для моделирования количества заявок
    for (unsigned int i = 0; i < this->getCount_of_requests(); i++)
    {
        this->source.create_request(&current_request.number_of_source, &current_request.time, this->current_time);
        this->setCurrent_time(current_request.getTime());
        time_of_moments.push_back(current_request);
    }

    //цикл по уникальным событиям(добавление заявки в буффер, добавление/освобождение заявки с прибора)
    while(not time_of_moments.empty())
    {

        //проверка текущего события(Заявка ли это?)
        if(time_of_moments[0].getStatus() == 0)
        {

            //добавление заявки в буффер
            this->source.count_of_request[time_of_moments[0].number_of_source]++;
            time_of_moments[0].number_of_request = static_cast<unsigned int>(source.count_of_request[time_of_moments[0].number_of_source]);
            this->stagingmanager.add_to_buffer(time_of_moments[0],
                                                this->buffer,
                                                &this->source,
                                                time_of_moments[0].getTime());

            // поиск свободного места в приборе
            bool free_place_in_device = false;
            for (unsigned int i = 0; i < this->getCount_of_divece(); i++)
            {
                if(this->device[this->selectionmanager.getPointer_on_push()].is_empty(time_of_moments[0].getTime())){
                    free_place_in_device = true;
                    break;
                }
                if(this->selectionmanager.getPointer_on_push() == this->getCount_of_divece() - 1)
                {
                    this->selectionmanager.setPointer_on_push(0);
                }else{
                    this->selectionmanager.pointer_on_push++;
                }
            }
            if(free_place_in_device)
            {

                //измение status у текущего события(добавление в заявки в буффер -> добавление заявки на прибор)
                time_of_moments[0].setStatus(1);

            }else {

                //удаление текущего события
                time_of_moments.pop_front();
            }
        }else{

            //вычисление загруженности приборов
            for (unsigned int i = 0; i < this->getCount_of_divece(); i++)
            {
                if((device[i].sum_time_working / time_of_moments[0].getTime()) * 100.0 < 100.0)
                {

                    device[i].operating_ratio =
                                                    (device[i].sum_time_working / time_of_moments[0].getTime()) * 100.0;

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
                unsigned int pop = this->selectionmanager.getPointer(this->selectionmanager.getPointer_on_push(), this->getCount_of_divece());
                time_of_moments[0].setTime(device[pop].getRelease_time());

                for (unsigned int i = 0; i < time_of_moments.size(); i++)
                {

                    for (unsigned int j = i; j < time_of_moments.size(); j++)
                    {

                        if(time_of_moments[i].getTime() > time_of_moments[j].getTime())
                        {

                            auto rec = time_of_moments[i];
                            time_of_moments[i] = time_of_moments[j];
                            time_of_moments[j] = rec;

                        }

                    }

                }
            }
        }
    }

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

    //Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("Кол-во заявок");
    horizontalHeader.append("Кол-во отказов");
    horizontalHeader.append("Время в сист.");
    horizontalHeader.append("Время ожидания");
    horizontalHeader.append("Время обработ.");
    horizontalHeader.append("Диспресия ожид.");
    horizontalHeader.append("Дисперсия обр.");
    horizontalHeader.append("Вер. отказа(%)");

    //Заголовки строк
    QStringList verticalHeader;
    for (unsigned int i = 0; i < this->getCount_of_sources(); i++)
    {
        verticalHeader.append("Источник " + QString::number(i+1));
    }

    model->setHorizontalHeaderLabels(horizontalHeader);
    model->setVerticalHeaderLabels(verticalHeader);

    for(int i = 0; i < static_cast<int>(this->getCount_of_sources()); i++)
    {
        item = new QStandardItem(QString::number(this->source.count_of_request[static_cast<unsigned int>(i)]));
        model->setItem(i, 0, item);

        item = new QStandardItem(QString::number(this->source.failed_requests[static_cast<unsigned int>(i)]));
        model->setItem(i, 1, item);

        item = new QStandardItem(QString::number(this->source.average_service_time[static_cast<unsigned int>(i)]
                + this->source.average_waiting_time[static_cast<unsigned int>(i)]));
        model->setItem(i, 2, item);

        item = new QStandardItem(QString::number(this->source.average_waiting_time[static_cast<unsigned int>(i)]));
        model->setItem(i, 3, item);

        item = new QStandardItem(QString::number(this->source.average_service_time[static_cast<unsigned int>(i)]));
        model->setItem(i, 4, item);

        item = new QStandardItem(QString::number(this->source.dispersion_waiting_time[static_cast<unsigned int>(i)]));
        model->setItem(i, 5, item);

        item = new QStandardItem(QString::number(this->source.dispersion_service_time[static_cast<unsigned int>(i)]));
        model->setItem(i, 6, item);

        item = new QStandardItem(QString::number(
        ((this->source.failed_requests[static_cast<unsigned int>(i)] * 100.0)/
                                 (this->source.count_of_request[static_cast<unsigned int>(i)] * 100.0)) * 100.0
                ));
        model->setItem(i, 7, item);

    }



    QStandardItemModel *model_1 = new QStandardItemModel;
    QStandardItem *item_1;

    //Заголовки столбцов
    QStringList horizontalHeader_1;
    horizontalHeader_1.append("Загруженность прибора(%)");


    //Заголовки строк
    QStringList verticalHeader_1;
    for (unsigned int i = 0; i < this->getCount_of_divece(); i++)
    {
        verticalHeader_1.append("Прибор " + QString::number(i+1));
    }

    model_1->setHorizontalHeaderLabels(horizontalHeader_1);
    model_1->setVerticalHeaderLabels(verticalHeader_1);

    for(int i = 0; i < static_cast<int>(this->getCount_of_divece()); i++)
    {
        item_1 = new QStandardItem(QString::number(this->device[static_cast<unsigned int>(i)].operating_ratio));
        model_1->setItem(i, 0, item_1);
    }



    this->ui->setupUi(this);
    setWindowTitle("Автоматический режим");
    this->ui->table_source->setModel(model);
    this->ui->table_device->setModel(model_1);

    this->ui->table_device->resizeRowsToContents();
    this->ui->table_device->resizeColumnsToContents();

    this->ui->table_source->resizeRowsToContents();
    this->ui->table_source->resizeColumnsToContents();
}

AutoMode::~AutoMode()
{
    delete ui;
}

void AutoMode::on_btn_Back_clicked()
{
    this->hide();
    parentWidget()->show();
}

double AutoMode::getBeta_device() const
{
    return beta_device;
}

void AutoMode::setBeta_device(double value)
{
    beta_device = value;
}

double AutoMode::getAlpha_device() const
{
    return alpha_device;
}

void AutoMode::setAlpha_device(double value)
{
    alpha_device = value;
}

double AutoMode::getLambda_source() const
{
    return lambda_source;
}

void AutoMode::setLambda_source(double value)
{
    lambda_source = value;
}

double AutoMode::getCurrent_time() const
{
    return current_time;
}

void AutoMode::setCurrent_time(double value)
{
    current_time = value;
}

unsigned int AutoMode::getCurrent_step() const
{
    return current_step;
}

void AutoMode::setCurrent_step(unsigned int value)
{
    current_step = value;
}

unsigned int AutoMode::getCurrent_number_request() const
{
    return current_number_request;
}

void AutoMode::setCurrent_number_request(unsigned int value)
{
    current_number_request = value;
}

unsigned int AutoMode::getBuffer_size() const
{
    return buffer_size;
}

void AutoMode::setBuffer_size(unsigned int value)
{
    buffer_size = value;
}

unsigned int AutoMode::getCount_of_requests() const
{
    return count_of_requests;
}

void AutoMode::setCount_of_requests(unsigned int value)
{
    count_of_requests = value;
}

unsigned int AutoMode::getCount_of_divece() const
{
    return count_of_divece;
}

void AutoMode::setCount_of_divece(unsigned int value)
{
    count_of_divece = value;
}

unsigned int AutoMode::getCount_of_sources() const
{
    return count_of_sources;
}

void AutoMode::setCount_of_sources(unsigned int value)
{
    count_of_sources = value;
}
