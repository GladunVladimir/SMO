#ifndef AUTOMODE_H
#define AUTOMODE_H

#include <QMainWindow>
#include "source.h"
#include "request.h"
#include "stagingmanager.h"
#include "selectionmanager.h"
#include <qwt_plot.h>
#include <deque>
namespace Ui {
class AutoMode;
}

class AutoMode : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoMode(QWidget *parent = nullptr, unsigned int count_of_sources = 0, unsigned int count_of_divece = 0, unsigned int count_of_requests = 0, unsigned int buffer_size = 0, double lambda_source = 0.0, double alpha_device = 0.0, double beta_device = 0.0);
    ~AutoMode();

    unsigned int getCount_of_sources() const;
    void setCount_of_sources(unsigned int value);

    unsigned int getCount_of_divece() const;
    void setCount_of_divece(unsigned int value);

    unsigned int getCount_of_requests() const;
    void setCount_of_requests(unsigned int value);

    unsigned int getBuffer_size() const;
    void setBuffer_size(unsigned int value);

    unsigned int getCurrent_number_request() const;
    void setCurrent_number_request(unsigned int value);

    unsigned int getCurrent_step() const;
    void setCurrent_step(unsigned int value);

    double getCurrent_time() const;
    void setCurrent_time(double value);

    double getLambda_source() const;
    void setLambda_source(double value);


    double getAlpha_device() const;
    void setAlpha_device(double value);

    double getBeta_device() const;
    void setBeta_device(double value);

private slots:

    void on_btn_Back_clicked();

private:
    Ui::AutoMode *ui;
    unsigned int count_of_sources = 0;
    unsigned int count_of_divece = 0;
    unsigned int count_of_requests = 0;
    unsigned int buffer_size = 0;
    unsigned int current_step = 0;
    unsigned int current_number_request = 1;
    double current_time = 0.0;
    double lambda_source = 0.0;
    double alpha_device = 0.0;
    double beta_device = 0.0;
    Source source;
    StagingManager stagingmanager;
    SelectionManager selectionmanager;
    Device device[100000];
    Request buffer[100000];
    QwtPlot *plot;

};

#endif // AUTOMODE_H
