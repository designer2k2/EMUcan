// EMUCan Lib Simple Test Example

// Example to be run on Arduino UNO R4 Wifi/Minima
// Configure the EMU Black to send the CAN Stream at 500KBPS

// Hint:
// Check the Clock on your MCP2515 Board, change MCP_8MHZ to fit.


// https://www.designer2k2.at
// Stephan Martin 02.07.2023

#include <Arduino_CAN.h>

#include "EMUcan.h"
// EMU initialized with base ID 600:
EMUcan emucan(0x600);

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  if (!CAN.begin(CanBitRate::BR_500k)) {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  Serial.println("------- CAN Read ----------");
}

void loop() {

  // Call the EMUcan lib with every received frame:
  if (CAN.available()) {
    CanMsg msg = CAN.read();
    emucan.checkEMUcan(msg.id, msg.data_length, msg.data);
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
