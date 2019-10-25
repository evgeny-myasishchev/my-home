#include <ArduinoUnit.h>
#include "logger.h"

void setup()
{
  // A0 should not be connected
  randomSeed(analogRead(0));

  Serial.begin(9600);
  while(!Serial) {} // Portability for Leonardo/Micro
  logger_setup(&Serial);

  // Test::exclude("*");
  // Test::include("dev_*");
  // Test::include("signal_io_*");
}

void loop()
{
  Test::run();
}