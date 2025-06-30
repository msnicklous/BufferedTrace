#include "Arduino.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#ifndef BufferedTrace_h
#define BufferedTrace_h

/**
 ** @copyright Copyright (C) 2025 Martin Scott Nicklous
 ** @class BufferedTrace
 ** @brief Project: BufferedTrace for Arduino 
 ** 
 ** @details The BufferedTrace class provides a wrapper to the Serial object that allows 
 ** trace information to be collected in a buffer and send it out through 
 ** the serial interface. 
 **
 ** This header file also provides a shared global object Trace of class BufferedTrace
 ** for convenience.
 **
*/

class BufferedTrace {
public:

  /**
   * @brief default constructor
   */
  BufferedTrace();

  /**
   * @brief Sets a stream other than the standard Serial
   * @details If you need to use a serial output stream other than the standard
   * Serial object, initialize your stream and pass it to this function 
   * before calling init().
   * @param serial - the serial stream to be used  
   */
  void setStream(Stream& serial);

  /**
   * @brief Initializes the serial interface for communication. 
   * @details The init method is meant to
   * be called from a central location, for example in the top-level sketch. If init
   * is not called, no tracing will be performed.
   * @param bufsize - the desired buffer size
  */
  void init(uint16_t bufsize=80);

  /**
   * @brief resets the trace buffer, and starts buffering trace info.
   */
  void open();

  /**
   * @brief sends the buffered information collected since the last open() call and resets the buffer.
   */
  void close();

  /**
   * @brief buffers information consisting of a string name and numeric value.
   * @details only works within an open - close bracket.
   * @param string - the name of the value to be displayed
   * @param val - the long value to be displayed
   */
  void trace(const char* string, long val);


  /**
   * @brief buffers information consisting of a string name and string value.
   * @details only works within an open - close bracket.
   * @param string - the name of the value to be displayed
   * @param val - the stringvalue to be displayed
   */
  void trace(const char* string, const char* val);


  /**
   * @brief buffers a string for output
   * @details only works within an open - close bracket.
   * @param string - the string to be displayed
   */
  void trace(const char* string);


  /**
   * @brief whether or not to flush the buffer when full
   * @details The autoFlush setting controls behavior when output is being buffered and the
   * buffer becomes full. If autoFlush is true, upon becoming full, the buffer will be
   * written to the serial string and emptied so that new information can be traced.
   * If autoFlush is false, any info added after the buffer is full will be discarded.
   * @param autoFlush - if true, the buffer is flushed. If false, overflow data will be discarded.
   */
  void setAutoFlush(bool autoFlush);


  /**
   * @brief Immediately outputs a string.
   * @details Writes a string to the serial interface without affecting the buffer.
   * @param string - the string to be transmitted.
   */
  void itrace(const char* string);

private:
  char* buffer = NULL;
  uint16_t buflen = 0;
  bool traceOn = false;
  bool isOpen = false;
  bool autoFlush = true;
  Stream& ser = Serial;

  // helper funtion to add string to buffer only if there is room
  void add2Buffer(const char* string);
};

// global trace object for convenience
extern BufferedTrace Trace;

// **********************
#endif
