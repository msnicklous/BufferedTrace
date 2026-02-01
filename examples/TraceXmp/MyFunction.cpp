#include "WString.h"
#include <BufferedTrace.h>
#include"MyFunction.h"

void doSomething() {
  Trace.trace("Did", "it");

  Trace.trace(F("PM Did"), "it again");

  Trace.trace(F("Hi from progmem!"));

  Trace.itrace(F("Immediate trace from PROGMEM!"));

  Trace.trace("ans", 42);

  Trace.trace(F("pm ans"), 84);
}
