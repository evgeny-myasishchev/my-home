#include "test-text-stream.h"
#include <string.h>

TestTextStream::TestTextStream()
{
    this->lastWrittenBuffer = 0;
    this->lastWrittenBufferLength = 0;

    this->bufferToRead = 0;
    this->bufferToReadLength = 0;
}

TestTextStream::~TestTextStream()
{
}

int TestTextStream::available()
{
    return this->bufferToReadLength;
}

size_t TestTextStream::read(char *buffer, size_t length)
{
    if(this->bufferToReadLength == 0)
    {
        return 0;
    }

    strncpy(buffer, this->bufferToRead, this->bufferToReadLength);
    auto result = this->bufferToReadLength;
    this->bufferToRead = 0;
    this->bufferToReadLength = 0;
    return result;
}

size_t TestTextStream::write(const char *buffer, size_t size)
{
    this->lastWrittenBuffer = buffer;
    this->lastWrittenBufferLength = size;
}

void TestTextStream::setReadBuffer(const char *buffer)
{
    this->bufferToRead = buffer;
    this->bufferToReadLength = strlen(buffer);
}
