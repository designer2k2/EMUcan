// EMUCan Lib Simple Send Example

// Example to be run on Arduino Nano with MCP2515
// Configure the EMU Black to send the CAN Stream at 500KBPS

// Configure custom CAN messages in the EMU Black:
// Use this file: EMUBlackCANStreamExample.canstr
// It will receive the values as CAN Analog 1 and 2.

// This MCP2515 Lib is used:
// https://github.com/autowp/arduino-mcp2515

// https://www.designer2k2.at
// Stephan Martin 28.12.2020

#include <EMUcan.h>

EMUcan emucan;

struct can_frame canMsg1;

unsigned long previousMillis = 0;
const long interval = 100;

byte countUp = 0;

void setup() {
  Serial.begin(115200);

  //Call this in the setup to init the lib:
  emucan.begin(CAN_500KBPS, MCP_8MHZ);

  // Frame to be send:
  canMsg1.can_id  = 0x0F6;
  canMsg1.can_dlc = 2;
  canMsg1.data[0] = 0xFF;
  canMsg1.data[1] = 0x00;

  Serial.println("------- CAN Send ----------");

}

void loop() {
  // This function should be called as often as possible
  emucan.checkEMUcan();

  // only send every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    countUp++;  //Byte, so overflow at 255
    canMsg1.data[0] = 255 - countUp;
    canMsg1.data[1] = countUp;

    //Sends the frame;
    emucan.sendFrame(&canMsg1);

    Serial.print("Message sent: ");
    Serial.println(countUp);

  }
}