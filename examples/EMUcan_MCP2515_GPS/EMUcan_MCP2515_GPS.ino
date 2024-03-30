// EMUCan Lib Test with GPS Example

// Example to be run on Arduino (Nano) with MCP2515
// Configure the EMU Black to send the CAN Stream at 500KBPS

// Hint:
// Check the Clock on your MCP2515 Board, change MCP_8MHZ to fit.

// This MCP2515 Lib is used:
// https://github.com/autowp/arduino-mcp2515


// https://www.designer2k2.at
// Stephan Martin 2024


#include "EMUcan.h"
// EMU initialized with base ID 600:
EMUcan emucan(0x600);

#include <mcp2515.h>
struct can_frame canMsg;
// MCP2515 with chip select (CS) pin 10:
MCP2515 mcp2515(10);

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  // Enable GPS:
  emucan.enableGPS(0x400);

  Serial.println("------- CAN Read ----------");

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Call the EMUcan lib with every received frame:
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    emucan.checkEMUcan(canMsg.can_id, canMsg.can_dlc, canMsg.data);
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
    if (emucan.EMUcan_GPS_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
      Serial.print(emucan.emu_data_gps.Latitude);
      Serial.print(";");
      Serial.print(emucan.emu_data_gps.Longitude);
      Serial.print(";");
      Serial.print(emucan.emu_data_gps.Height);
      Serial.print(";");
      Serial.print(emucan.emu_data_gps.Speed);
      Serial.print(";");
      Serial.println(emucan.emu_data_gps.GPS_status);
    } else {
      Serial.println("No communication from GPS");
    }
  }
}
