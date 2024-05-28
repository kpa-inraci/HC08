#include <Arduino.h>
#include "Hc08Config.h"

#if defined(__AVR__)
  Hc08Config hc08(10, 11); // RX, TX uniquement pour Arduino
#elif defined(ARDUINO_ARCH_ESP32)
  Hc08Config hc08(Serial1, 16, 17); // RX, TX pour ESP32, remappable
#elif defined(ARDUINO_ARCH_SAMD)
  Hc08Config hc08(Serial1); // Utilise Serial1 pour SAMD21
#else
  #error "Unsupported architecture"
#endif

void setup() {
  Serial.begin(9600); // Pour le moniteur série
  while (!Serial);

  hc08.begin(9600);

  hc08.testAT();
  hc08.setName("MyHC08");
  hc08.setPin("5678");
  hc08.setBaudRate(9600); // Utiliser des valeurs de baudrate typiques directement
}

void loop() {
  // Reste du code du projet
}
