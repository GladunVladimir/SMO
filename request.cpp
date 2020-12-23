#include "request.h"

Request::Request()
{

}

unsigned int Request::getStatus() const
{
    return status;
}

void Request::setStatus(unsigned int value)
{
    status = value;
}

bool Request::getIs_empty() const
{
    return is_empty;
}

void Request::setIs_empty(bool value)
{
    is_empty = value;
}

double Request::getTime() const
{
    return time;
}

void Request::setTime(double value)
{
    time = value;
}

unsigned int Request::getNumber_of_request() const
{
    return number_of_request;
}

void Request::setNumber_of_request(unsigned int value)
{
    number_of_request = value;
}

unsigned int Request::getNumber_of_source() const
{
    return number_of_source;
}

void Request::setNumber_of_source(unsigned int value)
{
    number_of_source = value;
}
