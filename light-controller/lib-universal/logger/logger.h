#ifndef LOGGER_H
#define LOGGER_H

// To disable define this somewhere before including logger.h
// #ifndef DISABLE_LOGGING

#include <arduino-compat.h>
#include <stdarg.h>
#include <Timers.h>

namespace logger
{
    #define DEC 10
    #define BIN 2

    class Output
    {
    public:
        virtual void print(const char*) = 0;
        virtual void print(char) = 0;
        virtual void print(int, int) = 0;
    };

    void printf(Output * out, const char *format, ...);

#ifdef ARDUINO
    class PrintOutput : public Output
    {
    private:
        Print * target;
    public:
        PrintOutput(Print*);

        void print(const char*);
        void print(char);
        void print(int, int);
    };
#endif

    class Logger
    {
    private:
        Output * output;
        Timers * timers;
    public:
        Logger(Timers * timers, Output * output);
        template<typename... Args> void log(const char * msg, Args... args) {
            logger::printf(this->output, "[INFO %d] ", this->timers->millis());
            logger::printf(this->output, msg, args...);
            printf(this->output, "\n");
        }
    };

    struct LoggingSystem {
        Logger * logger;
    };

    void setupLoggingSystem(Output *output);
    LoggingSystem * getLoggingSystem();
    // LoggingSystem * defaultLoggingSystem;
} // namespace logger

#ifndef DISABLE_LOGGING
#define logger_setup(out) logger::setupLoggingSystem(new logger::PrintOutput(out))
#define logger_log(msg, ...)  logger::getLoggingSystem()->logger->log(msg, ##__VA_ARGS__)
#else
#define logger_setup(out)
#define logger_log(msg, ...)
#endif

#endif