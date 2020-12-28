#ifndef LOGGER_H
#define LOGGER_H

// To disable define this somewhere before including logger.h
// #ifndef DISABLE_LOGGING

#include <arduino-compat.h>
#include <stdarg.h>
#include <Timers.h>
#include <rtc.h>

#ifndef ARDUINO
#include <iostream>
#define println(msg)  std::cout << msg << std::endl;
#endif

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
    #define println(msg)

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
        Timers *timers;
        rtc::Clock *clock;
    public:
        Logger(Timers *timers, rtc::Clock *clock, Output * output);
        template<typename... Args> void log(const char * msg, Args... args) {
            const auto now = clock->now();
            logger::printf(this->output, "[INFO %d:%d:%dT%d:%d:%d.%d] ", 
                now.year(), now.month(), now.day(),
                now.hour(), now.minute(), now.second(),
                this->timers->millis()
            );
            logger::printf(this->output, msg, args...);
            printf(this->output, "\n");
        }
    };

    struct LoggingSystem {
        Logger * logger;
    };

    void setupLoggingSystem(rtc::Clock *clock, Output *output);
    LoggingSystem * getLoggingSystem();
    // LoggingSystem * defaultLoggingSystem;
} // namespace logger

#ifndef DISABLE_LOGGING
#define logger_setup(clock, out) logger::setupLoggingSystem(clock, new logger::PrintOutput(out))
#define logger_log(msg, ...)  logger::getLoggingSystem()->logger->log(msg, ##__VA_ARGS__)
#else
#define logger_setup(clock, out)
#define logger_log(msg, ...)
#endif

#endif