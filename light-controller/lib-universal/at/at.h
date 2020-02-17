#ifndef AT_H
#define AT_H

#include <io.h>

namespace at
{

#ifdef AT_ENGINE_VERBOSE
#define at_engine_log logger_log
#else
#define at_engine_log
#endif

#define MAX_COMMAND_SIZE 30

#define RESPONSE_OK "OK\n"
#define RESPONSE_ERROR "ERROR\n"

class Input
{
public:
    char *body = 0;
    size_t length = 0;
    Input();
};

class Responder
{
private:
    io::TextStream *_stream;

public:
    Responder(io::TextStream *stream);

    void writeLine(const char *line);
    void writeOk();
    void writeError();
};

class Handler
{
private:
    const char *_name;

public:
    Handler(const char *name);
    virtual ~Handler();
    const char *Name();
    virtual void Handle(const Input input, Responder *resp) = 0;
};

class DefaultHandler : public Handler
{
public:
    DefaultHandler();
    void Handle(const Input input, Responder *resp);
};

class Engine
{
private:
    io::TextStream *_stream;
    Handler *_defaultHandler;
    Handler **_handlers = 0;
    size_t _handlersCount = 0;
    char _cmdBuffer[MAX_COMMAND_SIZE] = {};
    size_t _cmdBufferConsumed = 0;

    void resetBuffer();

public:
    Engine(io::TextStream *stream);
    ~Engine();

    void addCommandHandler(Handler *handler);
    void setup();
    void loop();
};

} // namespace at

#endif