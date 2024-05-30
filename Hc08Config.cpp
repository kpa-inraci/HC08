#include "Hc08Config.h"

#if defined(__AVR__) || defined(ARDUINO_ARCH_ESP32)
Hc08Config::Hc08Config(HardwareSerial& serial, int rxPin, int txPin)
    : hardwareSerial(&serial), rxPin(rxPin), txPin(txPin) {}

#elif defined(ARDUINO_ARCH_SAMD)
Hc08Config::Hc08Config(HardwareSerial& serial) : hardwareSerial(&serial) {}

#endif

void Hc08Config::begin(long baudRate) {
    #if defined(__AVR__)
        ((SoftwareSerial*)hardwareSerial)->begin(baudRate);
    #elif defined(ARDUINO_ARCH_ESP32)
        hardwareSerial->begin(baudRate, SERIAL_8N1, rxPin, txPin);
    #elif defined(ARDUINO_ARCH_SAMD)
        hardwareSerial->begin(baudRate);
    #endif
    delay(200); // Waiting for the module to be ready
}

void Hc08Config::sendCommand(const char* command, const char* expectedResponse) {
    hardwareSerial->println(command);
    unsigned long startTime = millis();
    const unsigned long timeout = 500; // in millisecond timeout

    while (millis() - startTime < timeout) {
        if (hardwareSerial->available()) {
            String response = hardwareSerial->readString();
            Serial.print("response : "); // Print response to Serial Monitor
            Serial.println(response); // Print response to Serial Monitor
            if (response.indexOf(expectedResponse) != -1) {
                return; // Expected response received, exit function
            }
        }
    }

    // If we reach here, it means the expected response was not received within the timeout period
    Serial.print("Error: No response or unexpected response received from -> command  ");
    Serial.println(command);
}

void Hc08Config::testAT() {
    sendCommand("AT", "OK");
}

void Hc08Config::setName(const char* name) {
    char command[20];
    snprintf(command, sizeof(command), "AT+NAME=%s", name);
    sendCommand(command, "OKsetNAME");
}

// Uncomment if you need these methods
// void Hc08Config::setPin(const char* pin) {
//     char command[20];
//     snprintf(command, sizeof(command), "AT+PIN=%s", pin);
//     sendCommand(command, "OKsetPIN");
// }

// void Hc08Config::setBaudRate(long baudRate) {
//     char command[20];
//     snprintf(command, sizeof(command), "AT+BAUD=%ld", baudRate);
//     sendCommand(command, "OKsetBAUD");
// }
  