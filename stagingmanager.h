#ifndef STAGINGMANAGER_H
#define STAGINGMANAGER_H
#include "request.h"
#include "source.h"

class StagingManager
{
public:
    StagingManager();
    ~StagingManager() = default;

    void add_to_buffer(Request item, Request *buffer, Source *source, double current_time);
    unsigned int getPointer(unsigned int pointer, unsigned int max_value);

    unsigned int pointer_on_add = 0;
    unsigned int pointer_on_failure = 0;
    unsigned int buffer_size;
    unsigned int getPointer_on_add() const;
    void setPointer_on_add(unsigned int value);
    unsigned int getBuffer_size() const;
    void setBuffer_size(unsigned int value);
    unsigned int getPointer_on_failure() const;
    void setPointer_on_failure(unsigned int value);
};

#endif // STAGINGMANAGER_H
