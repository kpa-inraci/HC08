#ifndef HC08CONFIG_H
#define HC08CONFIG_H

#include <Arduino.h>

class Hc08Config {
public:
    #if defined(__AVR__) || defined(ARDUINO_ARCH_ESP32)
        Hc08Config(HardwareSerial& serial, int rxPin, int txPin);
    #elif defined(ARDUINO_ARCH_SAMD)
        Hc08Config(HardwareSerial& serial);
    #else
        #error "Unsupported architecture"
    #endif

    void begin(long baudRate);
    void testAT();
    void setName(const char* name);
    // void setPin(const char* pin); // Uncomment if you need this
    // void setBaudRate(long baudRate); // Uncomment if you need this

private:
    HardwareSerial* hardwareSerial;
    int rxPin, txPin;
    void sendCommand(const char* command, const char* expectedResponse);
};

#endif // HC08CONFIG_H
