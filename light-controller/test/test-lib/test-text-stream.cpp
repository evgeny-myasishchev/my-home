#include "test-text-stream.h"

TestTextStream::TestTextStream()
{
}

TestTextStream::~TestTextStream()
{
}

int TestTextStream::available()
{
}

size_t TestTextStream::read(char *buffer, size_t length)
{
}

size_t TestTextStream::write(const char *buffer, size_t size)
{
    this->lastWrittenBuffer = buffer;
    this->lastWrittenBufferLength = size;
}
