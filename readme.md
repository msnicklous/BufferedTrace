# BufferedTrace - a simple Arduino library for debugging

by [Martin Scott Nicklous](https://github.com/msnicklous/)
| 
[Github Project Page](https://github.com/msnicklous/BufferedTrace/)

Released under the MIT License

---
## Overview
A common debugging technique is sending trace information to a console  
through the serial interface. Sometimes you need to collect information
from time-critical portions of your code and send it to the console later in order to minimize trace impact on program timing. 
This library helps you do that.

The Serial object (and probably most other serial stream implementations) has a buffer, and when you use Serial.println(), 
you write data into that buffer. The underlying Serial code will immediately start sending data over the serial interface.
In general, this will cause interrupts to fire, which can interfere with your time-critical code. When you use BufferedTrace,
transmission won't begin until your time-critical code has completed.

See [API Documentation](https://msnicklous.github.io/BufferedTrace/)

---

## Arduino Code

### Getting Started
To begin, include the BufferedTrace Library at the top of your sketch.

```
#include <BufferedTrace.h>
```

This will provide a global instance of the BufferedTrace class called Trace for your convenience. By default,
BufferedTrace uses the standard Serial stream for output, but a method is provided for using a different stream
if you need to. BufferedTrace needs to be initialized during the setup() function of your sketch, and since it 
uses Serial (or other stream), that needs to be initialized as well:

```
setup() {
  ...
  Serial.begin(115200);
  Trace.init();
  ...
}
```

If the Trace.init() function is not called, no tracing will be performed. Calls to other trace methods such as 
Trace.trace() and Trace.itrace() will be ignored.

BufferedTrace uses a default buffer size of 80 characters, but if you want a different size, you can specify that
in the init call:

```
setup() {
  ...
  Serial.begin(115200);
  Trace.init(120);
  ...
}
```

As a general tip, you usually want to keep your tracing brief, both to conserve memory as well as for timing reasons. 
It takes about 7 milliseconds to transfer 80 bytes, and for some applications, that delay can be significant.


### Buffered Tracing

To used buffered tracing, you bracket the Trace.trace() calls between the Trace.open() and Trace.close() calls.
Trace.open() resets the buffer and starts collecting trace data. Trace.close() causes all trace data collected by the Trace.trace() 
calls to be written to the serial interface. I usually put Trace.open() at the top of the loop() block and Trace.close()
at the end. 

```
void loop() {
  static int ref0 = analogRead(A0);
  long start = millis();
  
  Trace.open();

  if (digitalRead(SWITCH_PIN)) {
    ref0 = analogRead(A0);
    Trace.trace("ref", ref0);
  }
  
  // ... do interesting stuff ...
  
  long t = millis() - start;
  Trace.trace("time", t);

  Trace.close();
}
```
If you call Trace.trace() either before calling Trace.open() or after calling Trace.close(), any data will be ignored.

### Immediate Tracing

The Trace.itrace() function will immediately write a string to the serial interface without regards for contents
of the buffer.

```
void loop() {
  static int ref0 = analogRead(A0);
  long start = millis();
  
  Trace.open();

  if (digitalRead(SWITCH_PIN)) {
  	Trace.itrace("button pressed!!");
  }
  
  // ... do interesting stuff ...
  
  long t = millis() - start;
  Trace.trace("time", t);

  Trace.close();
}
```


