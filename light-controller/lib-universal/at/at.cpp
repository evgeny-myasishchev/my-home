#include <at.h>
#include <stdlib.h>
#include <string.h>
#include <logger.h>

namespace at
{

Input::Input()
{

}

Responder::Responder(io::TextStream *stream)
{
    this->_stream = stream;
}

void Responder::writeLine(const char *line)
{
    this->writeLine(line, strlen(line));
}

void Responder::writeLine(const char *line, const size_t length)
{
    // We need space for leading '+' and trailing '\n'
    auto outputSize = length + 2;
    char *output = (char *)malloc(outputSize);
    output[0] = '+';
    memcpy(&output[1], line, length);
    output[length + 1] = '\n';
    this->_stream->write(output, outputSize);
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

Handler::Handler(const char *name)
{
    _name = name;
}

Handler::~Handler()
{
}

const char *Handler::Name()
{
    return _name;
}

DefaultHandler::DefaultHandler() : Handler("AT")
{
}

void DefaultHandler::Handle(const Input input, Responder *resp)
{
    resp->writeOk();
}

Engine::Engine(io::TextStream *stream)
{
    _stream = stream;
    _defaultHandler = new DefaultHandler();
}

Engine::~Engine()
{
    free(_handlers);
    delete _defaultHandler;
}

void Engine::resetBuffer()
{
    memset(_cmdBuffer, 0, MAX_COMMAND_SIZE);
    _cmdBufferConsumed = 0;
}

void Engine::addCommandHandler(Handler *handler)
{
    at_engine_log("Registering AT handler: [%d]: %s", _handlersCount, handler->Name());

    auto nextHandlers = (Handler **)malloc(sizeof(Handler *) * (_handlersCount + 1));
    if (_handlersCount != 0)
    {
        memcpy(nextHandlers, _handlers, sizeof(Handler *) * _handlersCount);
    }
    nextHandlers[_handlersCount++] = handler;
    free(_handlers);
    _handlers = nextHandlers;
}

void Engine::setup()
{
    this->addCommandHandler(_defaultHandler);
    at_engine_log("AT engine initialized. Total handlers: %d", _handlersCount);
}

void Engine::loop()
{
    const size_t available = _stream->available();
    if (available > 0)
    {
        if (available + _cmdBufferConsumed > MAX_COMMAND_SIZE)
        {
            this->resetBuffer();
            Responder(_stream).writeError();
            return;
        }

        auto cmdSize = _stream->read(&_cmdBuffer[_cmdBufferConsumed], available);
        size_t cmdEnd = 0;
        size_t cmdInputStart = 0;
        bool cmdEndFound = false;
        for (size_t i = _cmdBufferConsumed; i < _cmdBufferConsumed + cmdSize; i++)
        {
            cmdEnd = i;
            if(_cmdBuffer[i] == '=')
            {
                cmdInputStart = i + 1;
            }
            if (_cmdBuffer[i] == '\n')
            {
                cmdEndFound = true;
                break;
            }
        }
        _cmdBufferConsumed = _cmdBufferConsumed + cmdSize;
        if (!cmdEndFound)
        {
            return;
        }
        // Ignore CR
        if (_cmdBuffer[cmdEnd - 1] == '\r')
        {
            cmdEnd = cmdEnd - 1;
        }

        Responder responder(_stream);

        auto handled = false;
        for (size_t i = 0; i < _handlersCount; i++)
        {
            auto handler = _handlers[i];
            auto name = handler->Name();

            auto cmdLength = cmdInputStart > 0 ? cmdInputStart - 1 : cmdEnd;
            if (strlen(name) == cmdLength && strncmp(name, _cmdBuffer, cmdLength) == 0)
            {
                handled = true;
                at::Input input;
                if(cmdInputStart > 0)
                {
                    input.body = &_cmdBuffer[cmdInputStart];
                    input.length = cmdEnd - cmdInputStart;
                }
                handler->Handle(input, &responder);
                break;
            }
        }

        if (!handled)
        {
            at_engine_log("Unexpected command: '%s'", _cmdBuffer);
            responder.writeError();
        }

        this->resetBuffer();
    }
}

} // namespace at