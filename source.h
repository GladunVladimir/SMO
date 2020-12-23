#ifndef SOURCE_H
#define SOURCE_H
#include <vector>

using namespace std;
class Source
{
public:
    Source();
    ~Source() = default;

    double DoubleRand(double _max, double _min);
    unsigned long factorial(unsigned long n); // нахождение факториала числа
    void data_init(unsigned int count_of_sources, double value); // установка интенсивнности источников
    void create_request(unsigned int *number_of_source, double *time, double current_time);

    vector <double> set_current_time();
    vector <double> vec_lambda;
    vector <vector<int>> vec_source_and_number;
    vector <int> count_of_request;
    vector <int> failed_requests;
    vector <double> sum_service_time;
    vector <double> sum_waiting_time;
    vector <int> count_of_serviced_request;
    vector <double> average_service_time;
    vector <double> average_waiting_time;
    vector <double> prob_of_failure;
    vector <double> dispersion_service_time;
    vector <double> dispersion_waiting_time;
};

#endif // SOURCE_H
