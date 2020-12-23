#include "stagingmanager.h"
#include <QDebug>

StagingManager::StagingManager()
{

}

void StagingManager::add_to_buffer(Request item, Request *buffer, Source *source, double current_time)
{

    bool flag_on_the_recording = false;
    for(unsigned int i = 0; i < this->getBuffer_size(); i++)
    {

        if (buffer[this->getPointer_on_add()].getIs_empty())
        {
            buffer[this->pointer_on_add].setNumber_of_request(item.number_of_request);
            buffer[this->pointer_on_add].setNumber_of_source(item.number_of_source);
            buffer[this->pointer_on_add].setTime(item.time);
            buffer[this->pointer_on_add].setIs_empty(false);

            flag_on_the_recording = true;
            source->dispersion_waiting_time[buffer[this->pointer_on_add].number_of_source] = (
                        current_time / source->count_of_request[buffer[this->pointer_on_add].number_of_source] );

        }

        if(this->pointer_on_add + 1 == this->getBuffer_size()){
            this->setPointer_on_add(0);
        }else{
            this->pointer_on_add++;
        }
        this->setPointer_on_failure(this->getPointer_on_add());

        if (flag_on_the_recording) break;
    }

    if (not flag_on_the_recording)
    {

        source->failed_requests[buffer[this->getPointer_on_failure()].number_of_source] += 1;
        source->prob_of_failure[buffer[this->getPointer_on_failure()].number_of_source] = (
                    (static_cast<double>(source->failed_requests[buffer[this->getPointer_on_failure()].number_of_source])
                / static_cast<double>(source->count_of_request[buffer[this->getPointer_on_failure()].number_of_source])) * 100.0);

        source->sum_waiting_time[buffer[this->getPointer_on_failure()].number_of_source] +=
                current_time - buffer[this->getPointer_on_failure()].getTime();
        source->average_waiting_time[buffer[this->getPointer_on_failure()].number_of_source] = (
                    source->sum_waiting_time[buffer[this->getPointer_on_failure()].number_of_source] /
                    source->count_of_request[buffer[this->getPointer_on_failure()].number_of_source]);


        buffer[this->getPointer_on_failure()].setNumber_of_request(item.number_of_request);
        buffer[this->getPointer_on_failure()].setNumber_of_source(item.number_of_source);
        buffer[this->getPointer_on_failure()].setTime(item.time);
        buffer[this->getPointer_on_failure()].setIs_empty(false);

        if(this->getPointer_on_failure() == this->getBuffer_size() - 1)
        {
            this->setPointer_on_failure(0);
        }else{
            this->pointer_on_failure++;
        }
        this->setPointer_on_add(this->getPointer_on_failure());
    }

}

unsigned int StagingManager::getPointer(unsigned int pointer, unsigned int max_value)
{

    if(pointer == 0)
    {
        return (max_value - 1);
    }else
    {
    return (pointer - 1);
    }
}

unsigned int StagingManager::getPointer_on_failure() const
{
    return pointer_on_failure;
}

void StagingManager::setPointer_on_failure(unsigned int value)
{
    pointer_on_failure = value;
}

unsigned int StagingManager::getBuffer_size() const
{
    return buffer_size;
}

void StagingManager::setBuffer_size(unsigned int value)
{
    buffer_size = value;
}

unsigned int StagingManager::getPointer_on_add() const
{
    return pointer_on_add;
}

void StagingManager::setPointer_on_add(unsigned int value)
{
    pointer_on_add = value;
}
