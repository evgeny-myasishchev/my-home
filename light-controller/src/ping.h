#if !defined(PING_H)
#define PING_H

#include <logger.h>

class Ping
{
private:
    long lastPingAgo = 0;
public:
    void loop();
};

void Ping::loop()
{
    const auto now = millis();
    if(now - lastPingAgo >= 1000) {
        lastPingAgo = now;
        logger_log("PING: %d", lastPingAgo);
    }
}


#endif // PING_H
