#include <ArduinoUnit.h>
#include <ArduinoLog.h>

void setup()
{
  // A0 should not be connected
  randomSeed(analogRead(0));

  Serial.begin(9600);
  while(!Serial) {} // Portability for Leonardo/Micro
  Log.begin (LOG_LEVEL_FATAL, &Serial);

  // Test::exclude("*");
  // Test::include("dev_*");
}

void loop()
{
  Test::run();
}