#include <at.h>
#include <stdlib.h>
#include <string.h>

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
    const char ok[] = "OK\n";
    this->_stream->write(ok, strlen(ok));
}

void Responder::writeError()
{
    const char ok[] = "ERROR\n";
    this->_stream->write(ok, strlen(ok));
}

} // namespace at