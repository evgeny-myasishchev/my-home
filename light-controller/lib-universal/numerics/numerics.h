#ifndef NUMERICS
#define NUMERICS

#include <stddef.h>
#include <ctype.h>
#include <logger.h>


struct ParsedNumber
{
    const int value;
    const bool valid;

    ParsedNumber(int value, bool valid): value(value), valid(valid) {};
};

ParsedNumber parseNumber(const char *buffer, const size_t length);

#endif