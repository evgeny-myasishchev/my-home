#ifndef MOCKS_TEST_TEXT_STREAM
#define MOCKS_TEST_TEXT_STREAM

#include <io.h>

class TestTextStream : public io::TextStream
{
public:
    const char *lastWrittenBuffer;
    size_t lastWrittenBufferLength;

    TestTextStream();
    ~TestTextStream();

    int available();
    size_t read(char *buffer, size_t length);
    size_t write(const char *buffer, size_t size);
};

#endif