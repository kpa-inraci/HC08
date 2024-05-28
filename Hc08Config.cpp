#include "Hc08Config.h"

#if defined(__AVR__)
  #include <SoftwareSerial.h>
#endif

#if defined(__AVR__)
Hc08Config::Hc08Config(int rxPin, int txPin) {
  softwareSerial = new SoftwareSerial(rxPin, txPin);
  btSerial = softwareSerial;
}
#elif defined(ARDUINO_ARCH_ESP32)
Hc08Config::Hc08Config(HardwareSerial& serialPort) : hardwareSerial(&serialPort), rxPin(-1), txPin(-1) {
  btSerial = hardwareSerial;
}

Hc08Config::Hc08Config(HardwareSerial& serialPort, int rxPin, int txPin) : hardwareSerial(&serialPort), rxPin(rxPin), txPin(txPin) {
  btSerial = hardwareSerial;
}
#else
Hc08Config::Hc08Config(HardwareSerial& serialPort) : hardwareSerial(&serialPort) {
  btSerial = hardwareSerial;
}
#endif

void Hc08Config::begin(long baudRate) {
  #if defined(__AVR__)
    softwareSerial->begin(baudRate);
  #elif defined(ARDUINO_ARCH_ESP32)
    if (rxPin != -1 && txPin != -1) {
      hardwareSerial->begin(baudRate, SERIAL_8N1, rxPin, txPin);
    } else {
      hardwareSerial->begin(baudRate);
    }
  #else
    hardwareSerial->begin(baudRate);
  #endif
  Serial.begin(baudRate);
}

void Hc08Config::setName(const char* name) {
  char command[50];
  sprintf(command, "AT+NAME%s", name);
  if (sendCommand(command, "OK")) {
    Serial.println("Nom du module changé avec succès");
  } else {
    Serial.println("Échec du changement de nom");
  }
}

void Hc08Config::setPin(const char* pin) {
  char command[50];
  sprintf(command, "AT+PIN%s", pin);
  if (sendCommand(command, "OK")) {
    Serial.println("PIN du module changé avec succès");
  } else {
    Serial.println("Échec du changement de PIN");
  }
}

void Hc08Config::setBaudRate(long rate) {
  int atRate;
  switch (rate) {
    case 1200:
      atRate = 1;
      break;
    case 2400:
      atRate = 2;
      break;
    case 4800:
      atRate = 3;
      break;
    case 9600:
      atRate = 4;
      break;
    case 19200:
      atRate = 5;
      break;
    case 38400:
      atRate = 6;
      break;
    case 57600:
      atRate = 7;
      break;
    case 115200:
      atRate = 8;
      break;
    default:
      Serial.println("Baudrate non supporté");
      return;
  }

  char command[50];
  sprintf(command, "AT+BAUD%d", atRate);
  if (sendCommand(command, "OK")) {
    Serial.println("Débit en bauds changé avec succès");
  } else {
    Serial.println("Échec du changement de débit en bauds");
  }
}

void Hc08Config::testAT() {
  if (sendCommand("AT", "OK")) {
    Serial.println("Module HC-08 est prêt");
  } else {
    Serial.println("Erreur de communication avec le module HC-08");
  }
}

bool Hc08Config::sendCommand(const char* command, const char* expectedResponse) {
  btSerial->println(command);
  delay(100);

  char response[50];
  strcpy(response, expectedResponse);

  if (btSerial->find(response)) {
    return true;
  } else {
    return false;
  }
}
