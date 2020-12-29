// EMUCan Lib Simple Send Example

// Example to be run on Arduino Nano with MCP2515
// Library hardcodes the 8MHZ Version
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

unsigned long previousMillis = 0;
const long interval = 100;

byte countUp = 0;

void setup() {
  Serial.begin(115200);

  //Call this in the setup to init the lib:
  emucan.begin();

  // Frame to be send:
  emucan.send_frame.can_id  = 0x0F6;
  emucan.send_frame.can_dlc = 2;
  emucan.send_frame.data[0] = 0xFF;
  emucan.send_frame.data[1] = 0x00;
  
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
    emucan.send_frame.data[0] = 255-countUp;
    emucan.send_frame.data[1] = countUp;

    //Sends the frame;
    emucan.sendFrame();
    
    Serial.println(emucan.emu_data.pulseWidth);
  }
}
