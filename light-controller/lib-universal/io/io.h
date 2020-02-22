#ifndef IO_H
#define IO_H

#include <arduino-compat.h>
#include <stddef.h>

namespace io
{

class TextStream
{
public:
    virtual int available() = 0;
    virtual size_t read(char *buffer, size_t length) = 0;
    virtual size_t write(const char *buffer, size_t size) = 0;
    virtual size_t write(const int number) = 0;
    virtual size_t write(const char value) = 0;
};

#ifdef ARDUINO

class SerialTextStream : public TextStream
{
private:
    Stream *_serial;

public:
    SerialTextStream(Stream *serial);

    int available();
    size_t read(char *buffer, size_t length);
    size_t write(const char *buffer, size_t size);
    size_t write(int number);
};

#endif

} // namespace io

#endif