// EMUCan Lib Simple Test Example

// Example to be run on Adafruit Feather M4 CAN Express (ATSAME51)
// Configure the EMU Black to send the CAN Stream at 500KBPS

// https://github.com/CraftWork7

#include <CANSAME5x.h>
CANSAME5x CAN;

#include "EMUcan.h"

// EMU initialized with base ID 600:
EMUcan emucan(0x600);

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  Serial.begin(115200);

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

  if (!CAN.begin(500E3)) {
    Serial.println("CAN.begin(...) failed.");
  }

  Serial.println("------- CAN Read ----------");
}

void loop() {

  // Call the EMUcan lib with every received frame:

  int packetSize = CAN.parsePacket();
  if (packetSize) {
    if ((!CAN.packetRtr()) and CAN.available()) {
      long id = CAN.packetId();
      uint8_t receivedData[packetSize];
      for (int i = 0; i < packetSize; i++) {
        receivedData[i] = CAN.read();
      }
      emucan.checkEMUcan(id, packetSize, receivedData);
    }

    // Serial out every second:
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (emucan.EMUcan_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
        Serial.print(emucan.emu_data.RPM);
        Serial.print(";");
        Serial.print(emucan.emu_data.TPS);
        Serial.print(";");
        Serial.print(emucan.emu_data.IAT);
        Serial.print(";");
        Serial.print(emucan.emu_data.CLT);
        Serial.print(";");
        Serial.print(emucan.emu_data.MAP);
        Serial.print(";");
        Serial.println(emucan.emu_data.pulseWidth);
      } else {
        Serial.println("No communication from EMU");
      }
      if (emucan.emu_data.flags1 & emucan.F_IDLE) {
        Serial.println("Engine Idle active");
      }
      if (emucan.decodeCel()) {
        Serial.println("WARNING Engine CEL active");
      }
    }
  }
}
