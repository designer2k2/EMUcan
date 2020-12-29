// EMUCan Lib Simple Test Example

// Example to be run on Arduino Nano with MCP2515
// Library hardcodes the 8MHZ Version
// Configure the EMU Black to send the CAN Stream at 500KBPS

// This MCP2515 Lib is used: 
// https://github.com/autowp/arduino-mcp2515


// https://www.designer2k2.at
// Stephan Martin 28.12.2020

#include <EMUcan.h>

EMUcan emucan;

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  Serial.begin(115200);

  //Call this in the setup to init the lib:
  emucan.begin();
  
  Serial.println("------- CAN Read ----------");

}

void loop() {
  // This function should be called as often as possible
  emucan.checkEMUcan();

  // only send every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print(emucan.emu_data.RPM);
    Serial.print(";");
    Serial.print(emucan.emu_data.TPS);
    Serial.print(";");
    Serial.print(emucan.emu_data.IAT);
    Serial.print(";");
    Serial.print(emucan.emu_data.MAP);
    Serial.print(";");
    Serial.println(emucan.emu_data.pulseWidth);
  }
}
