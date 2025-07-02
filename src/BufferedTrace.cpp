#include "Stream.h"
#include "BufferedTrace.h"
/**
 ** Trace module provides functions to send buffered trace information through the serial interface.
*/

BufferedTrace Trace;  // global convenience object

BufferedTrace::BufferedTrace() {
}

void BufferedTrace::setStream(Stream& serial) {
  ser = serial;
}

void BufferedTrace::init(uint16_t buflen) {

  // should not be called multiple times, but if so, avoid memory leaks
  free(buffer);
  this->buflen = 0;

  buffer = (char *)malloc(buflen);
  if (buffer == NULL) {
    traceOn = false;
    return;
  }
  this->buflen = buflen;
  memset(buffer, 0, buflen);
  traceOn=true;
}

void BufferedTrace::open() {
  if (!traceOn) return;
  isOpen = true;
  buffer[0] = '\0';
}

void BufferedTrace::close() {
  isOpen = false;
  if (!traceOn) return;

  if (strlen(buffer) > 0) {
    ser.println(buffer);
    ser.flush();
  }
  buffer[0] = '\0';
}

// resulting trace strings should be short. Long strings can impact performance.
void BufferedTrace::trace(const char* string, long val) {
  if (!traceOn || !isOpen) return;

  char tempbuf[14];  // a long can be max 11 chars
  sprintf(tempbuf, "%ld", val);

  if (strlen(buffer) != 0) {
    add2Buffer(", ");
  }
  add2Buffer(string);
  add2Buffer("=");
  add2Buffer(tempbuf);

}

// resulting trace strings should be short. Long strings can impact performance.
void BufferedTrace::trace(const char* string, const char* val) {
  if (!traceOn || !isOpen) return;

  if (strlen(buffer) != 0) {
    add2Buffer(", ");
  }
  add2Buffer(string);
  add2Buffer("=");
  add2Buffer(val);

}

// strings should be short.
void BufferedTrace::trace(const char* string) {
  if (!traceOn || !isOpen) return;
  if (strlen(buffer) != 0) {
    add2Buffer(", ");
  }
  add2Buffer(string);
}

// immediate trace ignoring buffer contents
void BufferedTrace::itrace(const char* string) {
  if (!traceOn) return;
  ser.flush();
  ser.println(string);
  ser.flush();
}

// autoflush
void BufferedTrace::setAutoFlush(bool af) {
  autoFlush = af;
}

// Adds string to buffer if there is room. If the buffer fills up while
// copying, if auto flush is active, buffer will be written out if it fills up.
// Otherwise, additional string data will just be ignored.
void BufferedTrace::add2Buffer(const char* string) {
  uint16_t ibuf = strlen(buffer);
  uint16_t istr = 0;
  const uint16_t tocopy = strlen(string);
  const uint16_t lastchar = buflen - 1;

  if (ibuf > lastchar) {
    itrace("big problem!");
    return;
  }

  while (istr < tocopy) {
    if (ibuf == lastchar) {
      if (autoFlush) {
        buffer[ibuf] = '\0';
        ser.println(buffer);
        strcpy(buffer, "**> ");
        ibuf = strlen(buffer);
      } else {
        break;
      }
    }
    buffer[ibuf++] = string[istr++];
  }
  buffer[ibuf] = '\0';
}
// **********************
