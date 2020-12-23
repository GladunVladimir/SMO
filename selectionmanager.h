#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H
#include "request.h"
#include "device.h"
#include <QList>
#include <QLabel>
#include "source.h"

class SelectionManager
{
public:
    SelectionManager();
    ~SelectionManager() = default;
    void add_to_device(Request *buffer, Device *device, double current_time, Source *source);
    unsigned int getPointer(unsigned int pointer, unsigned int max_value);

    unsigned int pointer_on_get = 0;
    unsigned int pointer_on_push = 0;
    unsigned int buffer_size;
    unsigned int count_of_device;
    unsigned int curr_prior = 0;

    unsigned int getPointer_on_get() const;
    void setPointer_on_get(unsigned int value);
    unsigned int getPointer_on_push() const;
    void setPointer_on_push(unsigned int value);
    unsigned int getBuffer_size() const;
    void setBuffer_size(unsigned int value);
    unsigned int getCount_of_device() const;
    void setCount_of_device(unsigned int value);
    unsigned int getCurr_prior() const;
    void setCurr_prior(unsigned int value);
};

#endif // SELECTIONMANAGER_H
