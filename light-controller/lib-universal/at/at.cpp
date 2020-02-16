#include <at.h>
#include <stdlib.h>
#include <string.h>
#include <logger.h>

namespace at
{

Responder::Responder(io::TextStream *stream)
{
    this->_stream = stream;
}

void Responder::writeLine(const char *line)
{
    // We need space for leading '+' and trailing '\n'
    char * output = (char*)malloc((strlen(line) + 2) *sizeof(char));
    strcat(output, "+");
    strcat(output, line);
    strcat(output, "\n");
    this->_stream->write(output, strlen(output));
    free(output);
}

void Responder::writeOk()
{
    this->_stream->write(RESPONSE_OK, strlen(RESPONSE_OK));
}

void Responder::writeError()
{
    this->_stream->write(RESPONSE_ERROR, strlen(RESPONSE_ERROR));
}

Engine::Engine(io::TextStream *stream)
{
    _stream = stream;
}

void Engine::loop()
{
    const size_t available = _stream->available();
    if(available > 0)
    {
        char cmdBuffer[MAX_COMMAND_SIZE] = {};
        auto commandSize = _stream->read(cmdBuffer, MAX_COMMAND_SIZE);
        Responder responder(_stream);
        if(strncmp("AT\n", cmdBuffer, commandSize) == 0)
        {
            responder.writeOk();
        }
        else
        {
            at_engine_log("Unexpected command: '%s'", cmdBuffer);
            responder.writeError();
        }
    }
}

} // namespace at