#include <io.h>

namespace io
{

#ifdef ARDUINO

SerialTextStream::SerialTextStream(Stream *serial)
{
    this->_serial = serial;
}

int SerialTextStream::available()
{
    return this->_serial->available();
}

size_t SerialTextStream::read(char *buffer, size_t length)
{
    return this->_serial->readBytes(buffer, length);
}

size_t SerialTextStream::write(const char *buffer, size_t size)
{
    return this->_serial->write(buffer, size);
}

#endif

}