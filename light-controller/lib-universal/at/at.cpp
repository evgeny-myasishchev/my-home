#include <at.h>
#include <stdlib.h>
#include <string.h>
#include <logger.h>

#include <iostream>

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

Handler::Handler(const char *name)
{
    _name = name;
}

const char* Handler::Name()
{
    return _name;
}

DefaultHandler::DefaultHandler() : Handler("AT")
{
}

void DefaultHandler::Handle(const char *input, Responder *resp)
{
    resp->writeOk();
}

Engine::Engine(io::TextStream *stream)
{
    _stream = stream;
    _defaultHandler = new DefaultHandler();
    this->addCommandHandler(_defaultHandler);
}

Engine::~Engine()
{
    free(_handlers);
    delete _defaultHandler;
}

void Engine::addCommandHandler(Handler *handler)
{
    at_engine_log("Registering AT handler: [%d]: %s", _handlersCount, handler->Name());

    auto nextHandlers = (Handler**) malloc(sizeof(Handler*) * (_handlersCount + 1));
    if(_handlersCount != 0)
    {
        memcpy(nextHandlers, _handlers, sizeof(Handler*) * _handlersCount);
    }
    nextHandlers[_handlersCount++] = handler;
    free(_handlers);
    _handlers = nextHandlers;
}

void Engine::loop()
{
    const size_t available = _stream->available();
    if(available > 0)
    {
        char cmdBuffer[MAX_COMMAND_SIZE] = {};
        auto commandSize = _stream->read(cmdBuffer, MAX_COMMAND_SIZE);

        size_t cmdEnd = 0;
        for (size_t i = 0; i < commandSize; i++)
        {
            cmdEnd = i;
            if(cmdBuffer[i] == '\n')
            {
                break;
            }
        }

        Responder responder(_stream);

        auto handled = false;
        for (size_t i = 0; i < _handlersCount; i++)
        {
            auto handler = _handlers[i];
            auto name = handler->Name();

            // std::cout << "Checking command:'" << name << "' got:'";
            // for (size_t j = 0; j < cmdEnd; j++)
            // {
            //     std::cout << cmdBuffer[j];
            // }
            // std::cout << "'" << std::endl;
            
            if(strlen(name) == cmdEnd && strncmp(name, cmdBuffer, cmdEnd) == 0)
            {
                handled = true;
                handler->Handle(0, &responder);
                break;
            }
        }
        
        if(!handled)
        {
            at_engine_log("Unexpected command: '%s'", cmdBuffer);
            responder.writeError();
        }
    }
}

} // namespace at