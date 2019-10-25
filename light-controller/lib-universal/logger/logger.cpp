#include "logger.h"

namespace logger
{
#ifdef ARDUINO
    PrintOutput::PrintOutput(Print* target) {
        this->target = target;
    };

    void PrintOutput::print(const char* msg) {
        this->target->print(msg);
    }

    void PrintOutput::print(char ch) {
        this->target->print(ch);
    }

    void PrintOutput::print(int value, int format) {
        this->target->print(value, format);
    }
#endif

    Logger::Logger(Timers * timers, Output * output) {
        this->timers = timers;
        this->output = output;
    }

    LoggingSystem * defaultLoggingSystem;

    void setupLoggingSystem(Output *output) {
        defaultLoggingSystem = new LoggingSystem();
        defaultLoggingSystem->logger = new Logger(new Timers(), output);
    }

    LoggingSystem * getLoggingSystem() {
        return defaultLoggingSystem;
    }

    void printf(Output *out, const char *format, ...)
    {
        va_list args;
        va_start(args, format);

        //
        // loop through format string
        for (; *format != 0; ++format)
        {
            if (*format == '%')
            {
                ++format;
                if (*format == '\0')
                    break;
                if (*format == '%')
                {
                    continue;
                }
                if (*format == 's')
                {
                    out->print(va_arg(args, char *));
                    continue;
                }
                if (*format == 'd' || *format == 'i')
                {
                    out->print((int)va_arg(args, int), DEC);
                    continue;
                }
                if (*format == 'b')
                {
                    out->print((int)va_arg(args, int), BIN);
                    continue;
                }
            }
            out->print((char)*format);
        }
    }
} // namespace logger