#include <ArduinoUnit.h>
#include <timers.h>

test(timers)
{
  Timers timers;
  const unsigned long now = timers.millis();
  assertEqual(now, millis());
}