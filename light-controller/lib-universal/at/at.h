#ifndef AT_H
#define AT_H

#include <io.h>

namespace at
{

#define COMMAND_HANDLER void (*handler)(const char *input, Responder *resp)
#define MAX_COMMAND_SIZE 30

#define RESPONSE_OK "OK\n"
#define RESPONSE_ERROR "ERROR\n"

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

class Engine
{
private:
    io::TextStream *_stream;

public:
    Engine(io::TextStream *stream);

    void addCommandHandler(const char *cmd, COMMAND_HANDLER);

    void loop();
};

} // namespace at

#endif