#ifndef REQUEST_H
#define REQUEST_H


class Request
{
public:
    Request();
    ~Request() = default;

    unsigned int number_of_source = 0;
    unsigned int number_of_request = 0;
    unsigned int status = 0;
    double time = 0;
    bool is_empty = true;

    unsigned int getNumber_of_source() const;
    void setNumber_of_source(unsigned int value);
    unsigned int getNumber_of_request() const;
    void setNumber_of_request(unsigned int value);
    double getTime() const;
    void setTime(double value);
    bool getIs_empty() const;
    void setIs_empty(bool value);
    unsigned int getStatus() const;
    void setStatus(unsigned int value);
};

#endif // REQUEST_H
