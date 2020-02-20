#include <stddef.h>
#include <ctype.h>
#include <logger.h>

struct ParsedNumber
{
    const int value;
    const bool valid;

    ParsedNumber(int value, bool valid): value(value), valid(valid) {};
};

ParsedNumber parseNumber(const char *buffer, const size_t length)
{
    int result = 0;
    bool valid = true;

    for (size_t i = 0; i < length; i++)
    {
        const char digit = buffer[i];
        if(isdigit(digit) != 0)
        {
            if(i > 0)
            {
                result *= 10;
            }
            const byte value = (byte)digit - 48;
            result += value;
        }
        else
        {
            valid = false;
            break;
        }
    }
    

    return ParsedNumber(result, valid);
};