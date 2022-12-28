// EMUCan Lib Simple Test Example

// Example to be run on Teensy (3 or 4)
// Configure the EMU Black to send the CAN Stream at 500KBPS

// https://www.designer2k2.at
// Stephan Martin 27.12.2022

#include "EMUcan.h"
// EMU initialized with base ID 600:
EMUcan emucan(0x600);

// CAN Things, CAN1 on pin22 pin23 is used at Teensy 4, and CAN0 on Teensy 3:
#include <FlexCAN_T4.h>
#if (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY40)) || (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41))
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
#else
FlexCAN_T4<CAN0, RX_SIZE_256, TX_SIZE_16> can1;
#endif

CAN_message_t canMsg;

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  Serial.println("------- CAN Read ----------");

  can1.begin();
  can1.setBaudRate(500000);

#if (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY40)) || (defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41))
  //on Teensy 4 64 works:
  can1.setMaxMB(64);
#else
  can1.setMaxMB(16);
#endif
  can1.enableFIFO();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Call the EMUcan lib with every received frame:
  if (can1.read(canMsg)) {
    emucan.checkEMUcan(canMsg.id, canMsg.len, canMsg.buf);
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
