#include "selectionmanager.h"
#include <QDebug>
#include <cmath>

SelectionManager::SelectionManager()
{

}

void SelectionManager::add_to_device(Request *buffer, Device *device, double current_time, Source *source)
{
    this->setCurr_prior(0);
    bool is_place_on_device = false;
    for(unsigned int i = 0; i < getCount_of_device(); i++)
    {
        if (device[i].is_empty(current_time))
        {
            is_place_on_device = true;
            this->setPointer_on_push(i);
            break;
        }
    }

    if (is_place_on_device)
    {
        bool is_place_in_buffer = false;
        for (unsigned int t = 0; t < this->getBuffer_size(); t++)
        {
            if (not buffer[t].getIs_empty())
            {
                is_place_in_buffer = true;
                break;
            }
        }
        if(is_place_in_buffer)
        {
            bool find_place = false;
            while(not find_place)
            {
                for(unsigned int j = 0; j < this->getBuffer_size(); j++)
                {
                    if ((not buffer[j].getIs_empty()) && (buffer[j].getNumber_of_source() == this->getCurr_prior()))
                    {
                        this->setPointer_on_get(j);
                        find_place = true;
                        break;
                    }
                    if(j == this->getBuffer_size() - 1)
                    {
                        this->curr_prior++;
                    }
                }
            }

            device[getPointer_on_push()].request_service(current_time);

            source->count_of_serviced_request[buffer[this->getPointer_on_get()].getNumber_of_source()] += 1;

            source->sum_service_time[buffer[this->pointer_on_get].number_of_source] += device[this->pointer_on_push].service_time;

            source->average_service_time.at(buffer[this->pointer_on_get].number_of_source) = (
                        source->sum_service_time[buffer[this->pointer_on_get].number_of_source]
                    / source->count_of_request[buffer[this->pointer_on_get].number_of_source]);

            source->sum_waiting_time[buffer[this->pointer_on_get].number_of_source] += current_time - buffer[this->pointer_on_get].time;
            source->average_waiting_time.at(buffer[this->pointer_on_get].number_of_source) = (
                        source->sum_waiting_time[buffer[this->pointer_on_get].number_of_source]
                    / source->count_of_request[buffer[this->pointer_on_get].number_of_source]);
            source->dispersion_service_time.at(buffer[this->pointer_on_get].number_of_source) = (
                        (current_time/source->count_of_serviced_request[buffer[this->pointer_on_get].number_of_source]));
            device[this->pointer_on_push].sum_time_working += device[this->pointer_on_push].service_time;


            device[this->getPointer_on_push()].setNumber_of_source(buffer[this->getPointer_on_get()].getNumber_of_source());
            device[this->getPointer_on_push()].setNumber_of_request(buffer[this->getPointer_on_get()].getNumber_of_request());
            buffer[this->getPointer_on_get()].setIs_empty(true);
            buffer[this->getPointer_on_get()].setTime(0.0);
            buffer[this->getPointer_on_get()].setNumber_of_source(0);
            buffer[this->getPointer_on_get()].setNumber_of_request(0);

        }
//        for(unsigned int j = 0; j < this->getBuffer_size(); j++)
//        {
//            if (not buffer[j].getIs_empty())
//            {

//                device[getPointer_on_push()].request_service(current_time);

//                source->count_of_serviced_request[buffer[this->getPointer_on_get()].getNumber_of_source()] += 1;

//                source->sum_service_time[buffer[this->pointer_on_get].number_of_source] += device[this->pointer_on_push].service_time;

//                source->average_service_time.at(buffer[this->pointer_on_get].number_of_source).push_back(
//                            source->sum_service_time[buffer[this->pointer_on_get].number_of_source]
//                        / source->count_of_request[buffer[this->pointer_on_get].number_of_source]);

//                source->sum_waiting_time[buffer[this->pointer_on_get].number_of_source] += current_time - buffer[this->pointer_on_get].time;
//                source->average_waiting_time.at(buffer[this->pointer_on_get].number_of_source).push_back(
//                            source->sum_waiting_time[buffer[this->pointer_on_get].number_of_source]
//                        / source->count_of_request[buffer[this->pointer_on_get].number_of_source]);
//                source->dispersion_service_time.at(buffer[this->pointer_on_get].number_of_source).push_back(
//                            (current_time/source->count_of_serviced_request[buffer[this->pointer_on_get].number_of_source]));
//                device[this->pointer_on_push].sum_time_working += device[this->pointer_on_push].service_time;


//                device[this->getPointer_on_push()].setNumber_of_source(buffer[this->getPointer_on_get()].getNumber_of_source());
//                device[this->getPointer_on_push()].setNumber_of_request(buffer[this->getPointer_on_get()].getNumber_of_request());
//                buffer[this->getPointer_on_get()].setIs_empty(true);
//                buffer[this->getPointer_on_get()].setTime(0.0);
//                buffer[this->getPointer_on_get()].setNumber_of_source(0);
//                buffer[this->getPointer_on_get()].setNumber_of_request(0);
//                is_place_in_buffer = true;
//                if (this->getPointer_on_push() == this->getCount_of_device() - 1){
//                    this->setPointer_on_push(0);
//                }else {
//                    this->pointer_on_push++;
//                }
//                break;
//            }else {
//                if(this->getPointer_on_get() == this->getBuffer_size() - 1){
//                    this->setPointer_on_get(0);
//                }else {
//                    this->pointer_on_get++;
//                }
//            }
//        }
//        if(this->getPointer_on_get() == this->getBuffer_size() - 1){
//            this->setPointer_on_get(0);
//        }else {
//            this->pointer_on_get++;
//        }
    }
}

unsigned int SelectionManager::getPointer(unsigned int pointer, unsigned int max_value)
{

    if(pointer == 0)
    {
        return (max_value - 1);
    }else
    {
        return (pointer - 1);
    }

}

unsigned int SelectionManager::getCurr_prior() const
{
    return curr_prior;
}

void SelectionManager::setCurr_prior(unsigned int value)
{
    curr_prior = value;
}

unsigned int SelectionManager::getCount_of_device() const
{
    return count_of_device;
}

void SelectionManager::setCount_of_device(unsigned int value)
{
    count_of_device = value;
}

unsigned int SelectionManager::getBuffer_size() const
{
    return buffer_size;
}

void SelectionManager::setBuffer_size(unsigned int value)
{
    buffer_size = value;
}

unsigned int SelectionManager::getPointer_on_push() const
{
    return pointer_on_push;
}

void SelectionManager::setPointer_on_push(unsigned int value)
{
    pointer_on_push = value;
}

unsigned int SelectionManager::getPointer_on_get() const
{
    return pointer_on_get;
}

void SelectionManager::setPointer_on_get(unsigned int value)
{
    pointer_on_get = value;
}
