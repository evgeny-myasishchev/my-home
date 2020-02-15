#ifndef AT_H
#define AT_H

#include <io.h>

namespace at
{

#define COMMAND_HANDLER void (*handler)(const char *input, Responder *resp)

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

class CommandHandler
{
private:
    io::TextStream *_stream;

public:
    CommandHandler(io::TextStream *stream);

    void addHandler(const char *cmd, COMMAND_HANDLER);

    void loop();
};

} // namespace at

#endif