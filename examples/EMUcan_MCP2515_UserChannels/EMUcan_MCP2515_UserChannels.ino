// EMUCan User Defined CAN Stream Example

// Example to be run on Arduino (Nano) with MCP2515

// Besides the default stream (base ID up to base ID +7) the EMU Black can
// send any of its internal channels in user defined CAN messages. These are
// configured in the EMU software under "User defined CAN stream", where every
// message has its own ID and carries channels described by the columns
// Type, Pos., Mult, Divider and Offset.
//
// As the content of these messages differs per installation, the library can
// not decode them on its own. Instead each channel is mapped once in setup()
// with addUserChannel() onto a float owned by the sketch:
//
//   emucan.addUserChannel(can_id, position, type, &target, mult, divider, offset);
//
// The arguments follow the columns of the EMU dialog:
//  - can_id:   the message ID. The first user defined message defaults to
//              base ID + 0x0F, so 0x60F with the default base of 0x600.
//  - position: the byte offset inside the frame, 0 to 7.
//  - type:     the source type: EMUcan::U8, S8, U16_LE, S16_LE, U16_BE, S16_BE.
//  - target:   the float that receives the value, updated with every
//              matching frame.
//  - mult, divider, offset: the scaling entered in the EMU, applied as
//              value = raw * divider / mult + offset. They default to 1, 1, 0.
//
// The frames are handed over with checkEMUcan() just like the default
// stream, no extra call is needed in loop(). Up to EMUCAN_USER_CHANNELS
// channels (8 by default) can be mapped, spread over any number of IDs.
//
// This example maps the two channels that the EMUcan_Simulator example sends
// on 0x60F, so it can be tried out without an ECU on the bus. With a real
// EMU, set up the same channels in the "User defined CAN stream" dialog, or
// adjust the addUserChannel() calls to your own setup.

// Configure the EMU Black to send the CAN Stream at 500KBPS

// Hint:
// Check the Clock on your MCP2515 Board, change MCP_8MHZ to fit.

// This MCP2515 Lib is used:
// https://github.com/autowp/arduino-mcp2515


// https://www.designer2k2.at


#include "EMUcan.h"
// EMU initialized with base ID 600:
EMUcan emucan(0x600);

#include <mcp2515.h>
struct can_frame canMsg;
// MCP2515 with chip select (CS) pin 10:
MCP2515 mcp2515(10);

unsigned long previousMillis = 0;
const long interval = 500;

// The user defined channels are stored in variables owned by the sketch:
float knockIgnCorrection;
float fuelTemperature;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  Serial.println("------- User defined CAN stream ----------");

  // Map the channels of message 0x60F, one call per channel.
  // addUserChannel returns false if the table is full or an argument is out
  // of range, so it is worth checking:

  // Knock ign correction in deg, 16 bits signed little endian at byte 0,
  // multiplier 10:
  if (!emucan.addUserChannel(0x60F, 0, EMUcan::S16_LE, &knockIgnCorrection, 10)) {
    Serial.println("Mapping the knock ign correction failed");
  }

  // Fuel temperature in deg C, 8 bits unsigned at byte 2, offset -40:
  if (!emucan.addUserChannel(0x60F, 2, EMUcan::U8, &fuelTemperature, 1, 1, -40)) {
    Serial.println("Mapping the fuel temperature failed");
  }

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Call the EMUcan lib with every received frame, it decodes the default
  // stream and the mapped user defined channels alike:
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    emucan.checkEMUcan(canMsg.can_id, canMsg.can_dlc, canMsg.data);
  }

  // Serial out every 500ms:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (emucan.EMUcan_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
      // The default stream is still available through emu_data, the user
      // defined channels are read straight from the mapped variables:
      Serial.print("RPM: ");
      Serial.print(emucan.emu_data.RPM);
      Serial.print(" Knock ign correction: ");
      Serial.print(knockIgnCorrection);
      Serial.print(" Fuel temperature: ");
      Serial.println(fuelTemperature);
    } else {
      Serial.println("No communication from EMU");
    }
  }
}
