#ifndef IO_H
#define IO_H

#include <stddef.h>

namespace io
{

class TextStream
{
public:
    virtual int available() = 0;
    virtual size_t read(char *buffer, size_t length) = 0;
    virtual size_t write(const char *buffer, size_t size) = 0;
};

} // namespace io

#endif