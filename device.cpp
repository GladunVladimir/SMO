#include "device.h"
#include "random"
#include <QDebug>

Device::Device()
{

}

void Device::request_service(double current_time)
{
    double r = this->DoubleRand(1,0);
    this->service_time = r * (this->getBeta() - this->getAlpha()) + this->getAlpha();
    this->release_time = this->service_time + current_time;
}

bool Device::is_empty(double current_time)
{
    return (this->release_time <= current_time);
}

double Device::DoubleRand(double _max, double _min)
{
    return _min + double(rand()) / RAND_MAX * (_max - _min);
}

double Device::getBeta() const
{
    return beta;
}

void Device::setBeta(double value)
{
    beta = value;
}

double Device::getAlpha() const
{
    return alpha;
}

void Device::setAlpha(double value)
{
    alpha = value;
}

double Device::getSum_time_working() const
{
    return sum_time_working;
}

void Device::setSum_time_working(double value)
{
    sum_time_working = value;
}

unsigned int Device::getNumber_of_request() const
{
    return number_of_request;
}

void Device::setNumber_of_request(unsigned int value)
{
    number_of_request = value;
}

unsigned int Device::getNumber_of_source() const
{
    return number_of_source;
}

void Device::setNumber_of_source(unsigned int value)
{
    number_of_source = value;
}

double Device::getService_time() const
{
    return service_time;
}

void Device::setService_time(double value)
{
    service_time = value;
}

double Device::getRelease_time() const
{
    return release_time;
}

void Device::setRelease_time(double value)
{
    release_time = value;
}
