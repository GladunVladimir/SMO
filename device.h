#ifndef DEVICE_H
#define DEVICE_H
#include <vector>

using namespace std;
class Device
{
public:
    Device();
    ~Device() = default;

    void request_service(double current_time);
    bool is_empty(double current_time);
    double DoubleRand(double _max, double _min);

    double alpha;
    double beta;
    double release_time = 0.0;
    double service_time = 0.0;
    unsigned int number_of_source = 0;
    unsigned int number_of_request = 0;
    double operating_ratio;
    double sum_time_working = 0.0;


    double getRelease_time() const;
    void setRelease_time(double value);
    double getService_time() const;
    void setService_time(double value);
    unsigned int getNumber_of_source() const;
    void setNumber_of_source(unsigned int value);
    unsigned int getNumber_of_request() const;
    void setNumber_of_request(unsigned int value);
    double getSum_time_working() const;
    void setSum_time_working(double value);

    double getAlpha() const;
    void setAlpha(double value);
    double getBeta() const;
    void setBeta(double value);
};

#endif // DEVICE_H
