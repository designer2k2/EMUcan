# EMUcan

ECUMaster EMU CAN Stream Reader Arduino Library

It reads the EMU CAN Stream and decodes it into something useful.

This works with any CAN enabled device, MCP2515, Teensy, ESP32, STM32, Arduino UNO R4 and more.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![arduino-library-badge](https://www.ardu-badge.com/badge/EMUcan.svg?)](https://www.ardu-badge.com/EMUcan)
[![CI badge](https://github.com/designer2k2/EMUcan/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/designer2k2/EMUcan/actions)
[![CI badge](https://github.com/designer2k2/EMUcan/actions/workflows/unit-test.yml/badge.svg)](https://github.com/designer2k2/EMUcan/actions)
[![Super-Linter](https://github.com/designer2k2/EMUcan/actions/workflows/super-linter.yml/badge.svg)](https://github.com/marketplace/actions/super-linter)

**Content:**

- [Installation](#installation)
- [Setup](#setup)
- [Hardware](#hardware)
- [Software usage](#software-usage)
  - [Initialization](#initialization)
  - [Check on CAN Bus updates](#check-on-can-bus-updates)
  - [Status](#status)
  - [Reading the Values](#reading-the-values)
  - [Reading Flags](#reading-flags)
  - [User defined CAN stream](#user-defined-can-stream)
  - [Sending Data](#sending-data-to-emu-black-via-can-bus)
- [Others](#others)
  - [Different Versions](#different-versions)
  - [Support](#support)
  - [Migrate](#migrate-from-version-below-2-or-the-emucant4)

## Installation

To install EMUcan into your Arduino IDE you can use the Library Manager (available from Arduino IDE version 1.6.2). Open the IDE and click to the `Sketch` menu and then `Include Library` → `Manage Libraries`. There search for "EMUcan" and click `Install`.

Or alternatively install it as ZIP file:

[Download](https://github.com/designer2k2/EMUcan/archive/main.zip) the ZIP file from our [repository](https://github.com/designer2k2/EMUcan/) and select in Arduino IDE `Sketch` → `Include library` → `Add .ZIP library...`.

Or clone it into your `Documents/Arduino/libraries` folder:

```sh
git clone https://github.com/designer2k2/EMUcan.git
```

When installed you will also see a few examples in `File` → `Examples` → `EMUcan` menu.

## Setup

In the EMU Black, set the CAN-Bus speed to 500 Kpbs and enable "Send EMU stream over CAN-Bus".

The CAN-Bus speed can be modified, see in the examples on how to do it depending on the hardware.

The EMU Stream base ID can be changed, the begin function takes this as parameter.

## Hardware

Wire up the Arduino/Teensy/ESP32/.. to the CAN Bus.

For the MCP2515: [Arduino-MCP2515](https://github.com/autowp/arduino-mcp2515#can-shield)

For ESP32 / Teensy: Tested CAN Bus Transceiver can be found here: [FlexCAN](https://github.com/PaulStoffregen/FlexCAN)

For the Arduino UNO R4 see the official Documentation: [R4 CAN](https://docs.arduino.cc/tutorials/uno-r4-wifi/can)

## Software usage

### Initialization

To start the library with EMU Can Base (600 by default)

```C++
EMUcan emucan(0x600);
```

You need to set up the receiving of CAN frames, see in the examples on how to to that. And then hand over the important parts to the EMUcan library.

### Check on CAN Bus updates

Call this for every received CAN frame:

```C++
emucan.checkEMUcan(can_id, can_dlc, data);
```

Where the `can_id` is the ID from the message. `can_dlc` is the data length and `data` the actual data.

For the MCP2515 this could look like:

```C++
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    emucan.checkEMUcan(canMsg.can_id, canMsg.can_dlc, canMsg.data);
  }
```

### Reading the Values

Example on how to read a value:

```C++
Serial.println(emucan.emu_data.RPM);
```

All the values:

see [Emucan.h](https://github.com/designer2k2/EMUcan/blob/main/src/EMUcan.h)

```C++
// Available data
struct emu_data_t {
  uint16_t RPM;                     //RPM
  uint16_t MAP;                     //kPa
  uint8_t TPS;                      //%
  int8_t IAT;                       //C
  float Batt;                       //V
  float IgnAngle;                   //deg
  float pulseWidth;                 //ms
  uint16_t Egt1;                    //C
  uint16_t Egt2;                    //C
  float dwellTime;                  //ms
  int8_t gear;                      //
  uint8_t Baro;                     //kPa
  float analogIn1;                  //V
  float analogIn2;                  //V
  float analogIn3;                  //V
  float analogIn4;                  //V
  float analogIn5;                  //V
  float analogIn6;                  //V
  int8_t emuTemp;                   //C
  float oilPressure;                //Bar
  uint8_t oilTemperature;           //C
  float fuelPressure;               //Bar
  int16_t CLT;                      //C
  float flexFuelEthanolContent;     //%
  float wboLambda;                  //λ
  uint16_t vssSpeed;                //km/h
  float lambdaTarget;               //λ
  uint16_t cel;                     //
  float LambdaCorrection;           //%
  uint8_t flags1;                   //Flags 1
  uint8_t outflags1;                //Outflags 1
  uint8_t outflags2;                //Outflags 2
  uint8_t outflags3;                //Outflags 3
  uint8_t outflags4;                //Outflags 4
  uint8_t pwm1;                     //%
  uint16_t boostTarget;             //kPa
  uint8_t pwm2;                     //%
  float fuel_used;                  //L
  uint8_t DSGmode;                  //DSG mode
  float DBWpos;                     //%
  float DBWtarget;                  //%
  uint16_t TCdrpmRaw;               //
  uint16_t TCdrpm;                  //
  uint8_t TCtorqueReduction;        //%
  uint8_t PitLimitTorqueReduction;  //%
};
```

### Reading Flags

Example on how to check if the Engine is currently in idle:

```C++
if (emucan.emu_data.flags1 & emucan.F_IDLE) {
  Serial.println("Engine Idle active");
}
```

The flags1 would contain following states:

```C++
enum FLAGS1 : uint8_t {
  F_GEARCUT = (1 << 0),          //1 - Gearcut active
  F_ALS = (1 << 1),              //1 - ALS active
  F_LC = (1 << 2),               //1 - Launch control active
  F_IDLE = (1 << 3),             //1 - Is in idle state
  F_TABLE_SET = (1 << 4),        //0 - table set 1, 1 - table set 2
  F_TC_INTERVENTION = (1 << 5),  //1 - traction control intervention
  F_PIT_LIMITER = (1 << 6),      //1 - Pit limiter active
  F_BRAKE_SWITCH = (1 << 7)      //1 - Brake switch active
};
```

There are many more registers, take a look at [EMUcan.h](https://github.com/designer2k2/EMUcan/blob/main/src/EMUcan.h)

The information if the CEL is on can be checked by a dedicated function:

```C++
if (emucan.decodeCel()){
  Serial.println("WARNING Engine CEL active");
}
```

Details on why the CEL is on is contained in the cel flag:

```C++
enum ERRORFLAG : uint16_t {
  ERR_CLT = (1 << 0),    //Coolant temperature sensor failed
  ERR_IAT = (1 << 1),    //IAT sensor failed
  ERR_MAP = (1 << 2),    //MAP sensor failed
  ERR_WBO = (1 << 3),    //Wide band oxygen sensor failed
  ERR_EGT1 = (1 << 4),   //EGT sensor #1 failed
  ERR_EGT2 = (1 << 5),   //EGT sensor #2 failed
  EGT_ALARM = (1 << 6),  //EGT too high
  KNOCKING = (1 << 7),   //Knock detected
  FFSENSOR = (1 << 8),   //Flex Fuel sensor failed
  ERR_DBW = (1 << 9),    //Drive by wire failure
  ERR_FPR = (1 << 10)    //Fuel pressure relative error
};
```

Example to check CEL against the ERR_CLT:

```C++
if (emucan.emu_data.cel & emucan.ERR_CLT) {
  Serial.println("WARNING Engine CEL active due to CLT");
}
```

### User defined CAN stream

Besides the default stream the EMU can send any of its internal channels in **user defined CAN messages**, configured in the EMU software under "User defined CAN stream". Every message has its own ID, and carries channels described by Type, Pos., Mult, Divider and Offset. The default ID of the first message is the EMU stream base ID plus 0x0F, so 0x60F with the default base.

Such a channel is mapped onto a float of your own with `addUserChannel`, one call per channel:

```C++
// The decoded values live in your own variables:
float knockIgnCorrection, fuelTemp, boost;

void setup() {
  // Message 0x60F: Knock ign correction, 16 bits signed little endian, Pos. 0, Mult. 10
  emucan.addUserChannel(0x60F, 0, EMUcan::S16_LE, &knockIgnCorrection, 10);

  // Any other channel, on any number of message IDs:
  emucan.addUserChannel(0x610, 0, EMUcan::U8, &fuelTemp, 1, 1, -40);
  emucan.addUserChannel(0x610, 2, EMUcan::U16_LE, &boost);
}
```

The frames are handed over with `checkEMUcan` just like the default stream, and the values are read from the variables:

```C++
Serial.println(knockIgnCorrection);  //deg, negative means the ignition got pulled
```

The arguments follow the columns of the EMU dialog:

- `can_id` is the message ID.
- `position` is the **byte** offset inside the frame, 0 to 7.
- `type` is the source type, one of `EMUcan::U8`, `EMUcan::S8`, `EMUcan::U16_LE`, `EMUcan::S16_LE`, `EMUcan::U16_BE` and `EMUcan::S16_BE`. Signed types keep their sign, 32 bit types are not supported as a float can not hold them exactly.
- `target` is the float that receives the value. It is set to 0 while it is mapped, and updated with every matching frame.
- `mult`, `divider` and `offset` are the scaling entered in the EMU, applied as `value = raw * divider / mult + offset`.

Up to `EMUCAN_USER_CHANNELS` channels can be mapped, 8 by default, spread over as many message IDs as wanted. Raise it from the build with `-DEMUCAN_USER_CHANNELS=16` if more are needed. `addUserChannel` returns false if the table is full or an argument is out of range, so it is worth checking.

User defined values are deliberately not part of `emu_data`, that struct mirrors the default stream which looks the same on every EMU.

The `EMUcan_MCP2515_UserChannels` example shows the feature in a complete sketch, and the `EMUcan_Simulator` example sends matching frames on 0x60F, so it can be tried out without an ECU on the bus.

### Status

The EMUcan library provides its status:

```C++
enum EMUcan_STATUS {
  EMUcan_FRESH,
  EMUcan_RECEIVED_WITHIN_LAST_SECOND,
  EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND,
};
```

Reading the status:

```C++
if (emucan.EMUcan_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
    Serial.println("Data from EMU received");
} else {
    Serial.println("No communication from EMU");
}
```

### Sending Data to EMU Black (via CAN Bus)

The following examples demonstrate how to format and send CAN bus messages compatible with the EMU Black ECU, using common hardware platforms. While these examples reside within the EMUcan project repository, they primarily illustrate the correct CAN message structure for sending data _to_ the EMU, which may not strictly require the `EMUcan` library itself for the transmission part.

- **Sending Analog Values (Example with MCP2515):**
  The [EMUcan_MCP2515_send](https://github.com/designer2k2/EMUcan/tree/main/examples/EMUcan_MCP2515_send) sketch shows how to send an analog value (e.g., a sensor reading) using an external MCP2515 CAN controller. It demonstrates the required CAN ID and data formatting.

- **Sending Digital Values (Example with ESP32):**
  The [EMUcan_ESP32_send](https://github.com/designer2k2/EMUcan/tree/main/examples/EMUcan_ESP32_send) sketch shows how to send a digital value (e.g., a switch state) using the ESP32's built-in CAN controller, again illustrating the necessary CAN message format.

These examples provide a practical guide for constructing and transmitting the CAN messages that the EMU Black expects to receive. You would typically use a standard CAN library appropriate for your hardware (like the ESP32 CAN (TWAI) driver or an MCP2515 library) to perform the actual transmission shown in these examples.

## Others

This Library is tested on Arduino Nano with a MCP2515 shield at 8Mhz.

Further on a Teensy4, Teensy3 and ESP32 with SN65HVD232 transceiver.

The EMU Black was running Software Version 2.154.

### Different Versions

For using the ECUMaster serial stream instead of the CAN Bus use this library: [EMUSerial](https://github.com/GTO2013/EMUSerial)

### Migrate from version below 2, or the EMUcanT4

before Version 2, or in the EMUcanT4, the CAN Bus handling was part of the EMUcan lib. Now from Version 2 onwards you have to basically set this up by yourself and handover the CAN frame into the EMUcan library.

This gives you full control over the CAN interface, it also enables this library to be run on basically every hardware.

Please see in the examples on how to make this happen on MCP2515, Teensy and ESP32.

### Support

Please feel free to use/extend/report bugs/request features!
Take a look at the [Contribution Documents](https://github.com/designer2k2/EMUcan/blob/main/CONTRIBUTING.md) to see how to it.
