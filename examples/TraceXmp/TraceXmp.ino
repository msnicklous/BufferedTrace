#include <BufferedTrace.h>
#include "MyFunction.h"

#define BAUD 115200

void setup() {
  Serial.begin(BAUD);
  Trace.itrace("I'm here!");
  Trace.init();
  Trace.itrace("I'm here!");
}

void loop() {
  Trace.trace("won't get printed, trace not open.");
  Trace.open();

  Trace.trace("ans", 42);
  
  Trace.itrace("\nNote that this appears before the buffered output!");

  doSomething();

  Trace.trace("rand", random());

  Trace.close();

  delay(872);
}
