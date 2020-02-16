#ifndef TEST_AT_HANDLER_H
#define TEST_AT_HANDLER_H

#include <at.h>

class TestATHandler : public at::Handler
{
private:
    const char *_response;
public:
    bool called = false;
    const char *gotInput = 0;
    TestATHandler(const char *name, const char *response) : at::Handler(name)
    {
        _response = response;
    }

    void Handle(const char *input, at::Responder *resp)
    {
        called = true;
        gotInput = input;
        resp->writeLine(_response);
        resp->writeOk();
    }
};

#endif