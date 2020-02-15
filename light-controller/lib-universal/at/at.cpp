#include <at.h>

namespace at
{

Responder::Responder(io::TextStream *stream)
{
    this->_stream = stream;
}

void Responder::writeLine(const char *line)
{
}

void Responder::writeOk()
{
    const char ok[] = "OK\n";
    this->_stream->write(ok, sizeof(ok));
}

void Responder::writeError()
{
    const char ok[] = "ERROR\n";
    this->_stream->write(ok, sizeof(ok));
}

} // namespace at