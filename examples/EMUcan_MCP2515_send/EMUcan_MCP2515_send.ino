// EMUCan Lib Test Example with sending to the EMU

// Example to be run on Arduino (Nano) with MCP2515

// Configure the EMU Black to send the CAN Stream at 500KBPS

// Configure custom CAN messages in the EMU Black:
// Use this file: EMUBlackCANStreamExample.canstr
// It will receive the values as CAN Analog 1 and 2.

// This MCP2515 Lib is used:
// https://github.com/autowp/arduino-mcp2515


// https://www.designer2k2.at
// Stephan Martin 27.12.2022


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

  Serial.println("------- CAN Read ----------");

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
}

void send_to_emu() {
  // This sends a frame to the emu with the MCP2515:
  struct can_frame canMsg1;

  // Frame to be send:
  canMsg1.can_id = 0x0F6;
  canMsg1.can_dlc = 2;
  canMsg1.data[0] = 0xFF;
  canMsg1.data[1] = 0x00;

  // Send frame:
  mcp2515.sendMessage(&canMsg1);
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

      // Send frame:
      send_to_emu();

      // Status from EMU:
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