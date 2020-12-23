#include <vector>
#include <random>
#include <QDebug>
#include <QString>
#include <vector>
#include <math.h>
#include "source.h"
#include <random>

using namespace std;
Source::Source()
{

}


double Source::DoubleRand(double _max, double _min)
{
    return _min + double(rand()) / RAND_MAX * (_max - _min);
}


unsigned long Source::factorial(unsigned long n)
{
    if(n == 0)
    {
        return 1;
    }else
    {
        return n * factorial(n - 1);
    }
}


void Source::data_init(unsigned int count_of_sources, double value)
{
    for (unsigned int i = 0; i < count_of_sources; i++)
    {
        count_of_request.push_back(0);
        vec_lambda.push_back(value);

        sum_service_time.push_back(0.0);
        sum_waiting_time.push_back(0.0);
        count_of_serviced_request.push_back(0);
        failed_requests.push_back(0);


        prob_of_failure.push_back(0.0);
        dispersion_service_time.push_back(0.0);
        dispersion_waiting_time.push_back(0.0);
        average_service_time.push_back(0.0);
        average_waiting_time.push_back(0.0);
    }
}


void Source::create_request(unsigned int *number_of_source, double *time, double current_time)
{
    double min_time = 100000000000.0;
    unsigned int source_num = 0;
    double time_ = 0.0;
    for (unsigned int i = 0; i < this->vec_lambda.size(); i++)
    {
        double r = DoubleRand(1, 0);
        time_ = (-1/vec_lambda.at(i)) * std::log(r) + current_time;
        if (time_ < min_time)
        {
            min_time = time_;
            source_num = i;
        }
    }
    *number_of_source = source_num;
    *time = min_time;
}

vector<double> Source::set_current_time()
{
    vector<double> current_time;
    for (unsigned int i = 0; i < vec_lambda.size(); i++)
    {
        current_time.push_back(0.0);
    }
    return current_time;
}




