// EMUCan Lib Advanced Test Example

// Example to be run on Arduino Nano with MCP2515
// Configure the EMU Black to send the CAN Stream at 500KBPS

// This MCP2515 Lib is used:
// https://github.com/autowp/arduino-mcp2515


// https://www.designer2k2.at
// Stephan Martin 28.12.2020

#include <EMUcan.h>

//Define the EMU Can Base at 600, and the CS Pin from the MCP215 at 10):
EMUcan emucan(600, 10);

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(115200);

  //Call this in the setup to init the lib:
  emucan.begin(CAN_500KBPS, MCP_8MHZ);

  Serial.println("------- CAN Read ----------");

}

void loop() {
  // This function should be called as often as possible
  emucan.checkEMUcan();

  // only send every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (emucan.EMUcan_Status == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
      Serial.println(emucan.emu_data.RPM);
      // Check the FLAGS1 if the Engine is Idle:
      if (emucan.emu_data.flags1 & emucan.F_IDLE) {
        Serial.println("Engine Idle active");
      }
    } else {
      Serial.println("No communication from EMU");
    }

  }
}
