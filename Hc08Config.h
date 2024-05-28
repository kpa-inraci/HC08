#ifndef HC08CONFIG_H
#define HC08CONFIG_H

#include <Arduino.h>

class Hc08Config {
  public:
    #if defined(__AVR__)
      Hc08Config(int rxPin, int txPin);
    #elif defined(ARDUINO_ARCH_ESP32)
      Hc08Config(HardwareSerial& serialPort);
      Hc08Config(HardwareSerial& serialPort, int rxPin, int txPin);
    #else
      Hc08Config(HardwareSerial& serialPort);
    #endif

    void begin(long baudRate);
    void setName(const char* name);
    void setPin(const char* pin);
    void setBaudRate(long rate);
    void testAT();

  private:
    Stream* btSerial;
    #if defined(__AVR__)
      SoftwareSerial* softwareSerial;
    #else
      HardwareSerial* hardwareSerial;
      #if defined(ARDUINO_ARCH_ESP32)
        int rxPin, txPin;
      #endif
    #endif
    bool sendCommand(const char* command, const char* expectedResponse);
};

#endif
