# EMUcan
ECUMaster EMU CAN Stream Reader Arduino Library

It reads the EMU CAN Stream and decodes it into something usefull.

It also can send data to the EMU!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/EMUcan.svg?)](https://www.ardu-badge.com/EMUcan)

**Content:**

* [Installation](#installation)
* [Setup](#setup)
* [Hardware](#hardware)
* [Software usage](#software-usage)
   * [Initialization](#initialization)
   * [Check on CAN Bus updates](#check-on-can-bus-updates)
   * [Status](#status)
   * [Reading the Values](#reading-the-values)
   * [Reading Flags](#reading-flags)
   * [Sending Data](#sending-data)
   * [Custom Frame Receive](#custom-frame-receive)
* [Others](#others)
   * [Different Versions](#different-versions)
   * [Support](#support)

## Installation

To install EMUcan into your Arduino IDE you can use the Library Manager (available from Arduino IDE version 1.6.2). Open the IDE and click to the `Sketch` menu and then `Include Library` → `Manage Libraries`. There search for "EMUcan" and click `Install`.

Or alternatively install it as ZIP file:

[Download](https://github.com/designer2k2/EMUcan/archive/main.zip) the zip file from our [repository](https://github.com/designer2k2/EMUcan/) and select in Arduino IDE `Sketch` → `Include library` → `Add .ZIP library...`.

Or clone it into your `Documents/Arduino/libraries` folder:

```sh
git clone https://github.com/designer2k2/EMUcan.git
```

When installed you will also see a few examples in `File` → `Examples` → `EMUcan` menu.

This library requires the following MCP2515 library:

MCP2515 Library: https://github.com/autowp/arduino-mcp2515

## Setup

In the EMU Black, set the CAN-Bus speed to 500 Kpbs and enable "Send EMU stream over CAN-Bus".

The EMU Stream base ID can be changed, the begin function takes this as parameter.

CAN-Bus speed, and MCP2515 Clock speed can be set too, look in the examples.

## Hardware

Wire up the Arduino as shown in the MCP2515 library: https://github.com/autowp/arduino-mcp2515#can-shield

# Software usage

## Initialization

To create connection with the EMU Can Base (600 by default) and the MCP2515 pin number where SPI CS is connected (10 by default).

```C++
EMUcan emucan(0x600, 10);
```

Then in the begin define the CAN Bus baudrate and the MCP2515 clock speed

```C++
emucan.begin(CAN_500KBPS, MCP_8MHZ);
```

See the MCP2515 library for available baudrates and clock speeds: https://github.com/autowp/arduino-mcp2515#initialization

## Check on CAN Bus updates

Call this as often as possible in the loop function:

```C++
emucan.checkEMUcan();
```

## Status

The EMUcan library provides its status:

```C++
enum EMUcan_STATUS {
  EMUcan_FRESH,
  EMUcan_RECEIVED_WITHIN_LAST_SECOND,
  EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND,
};
```

## Reading the Values

Example on how to read a value:
```C++
Serial.println(emucan.emu_data.RPM);
```

All the values:

see https://github.com/designer2k2/EMUcan/blob/main/src/EMUcan.h

```C++
// Available data
struct emu_data_t {
  uint16_t RPM;  //RPM
  uint16_t MAP;  //kPa
  uint8_t TPS;  //%
  int8_t IAT;  //C
  float Batt;  //V
  float IgnAngle;  //deg
  float pulseWidth;  //ms
  float scondarypulseWidth;  //ms
  uint16_t Egt1;  //C
  uint16_t Egt2;  //C
  float knockLevel;  //V
  float dwellTime;  //ms
  float wboAFR;  //AFR
  int8_t gear;  //
  uint8_t Baro;  //kPa
  float analogIn1;  //V
  float analogIn2;  //V
  float analogIn3;  //V
  float analogIn4;  //V
  float analogIn5;  //V
  float analogIn6;  //V
  float injDC;  //%
  int8_t emuTemp;  //C
  float oilPressure;  //Bar
  uint8_t oilTemperature;  //C
  float fuelPressure;  //Bar
  int16_t CLT;  //C
  float flexFuelEthanolContent;  //%
  int8_t ffTemp;  //C
  float wboLambda;  //λ
  uint16_t vssSpeed;  //km/h
  uint16_t deltaFPR;  //kPa
  uint8_t fuelLevel;  //%
  uint8_t tablesSet;  //
  float lambdaTarget;  //λ
  float afrTarget;  //AFR
  uint16_t cel;  //
  float LambdaCorrection; //%
  uint8_t flags1; //Flags 1
  uint8_t outflags1; //Outflags 1
  uint8_t outflags2; //Outflags 2
  uint8_t outflags3; //Outflags 3
  uint8_t outflags4; //Outflags 4
  uint8_t pwm1; //%
  uint16_t boostTarget; //kPa
};
```

## Reading Flags

Example on how to check if the Engine is currently in idle:

```C++
if (emucan.emu_data.flags1 & emucan.F_IDLE) {
  Serial.println("Engine Idle active");
}
```

The flags1 would contain following states:
```C++
enum FLAGS1 : uint8_t {
  F_GEARCUT = (1 << 0),
  F_ALS = (1 << 1),
  F_LC = (1 << 2),
  F_IDLE = (1 << 3),
  F_TABLE_SET = (1 << 4),
  F_TC_INTERVENTION = (1 << 5),
  F_PIT_LIMITER = (1 << 6),
  F_BRAKE_SWITCH = (1 << 7)
};
```

There are many more registers, take a look at https://github.com/designer2k2/EMUcan/blob/main/src/EMUcan.h 

The information if the CEL is on can be checked by a dedicated function:

```C++
if (emucan.decodeCel()){
  Serial.println("WARNING Engine CEL active");
}
```

Details on why the CEL is on is contained in the cel flag:

```C++
enum ERRORFLAG : uint16_t {
  ERR_CLT = (1 << 0),
  ERR_IAT = (1 << 1),
  ERR_MAP = (1 << 2),
  ERR_WBO = (1 << 3),
  ERR_EGT1 = (1 << 4),
  ERR_EGT2 = (1 << 5),
  EGT_ALARM = (1 << 6),
  KNOCKING = (1 << 7),
  FFSENSOR = (1 << 8),
  ERR_DBW = (1 << 9),
  ERR_FPR = (1 << 10)
};
```

Example to check CEL against the ERR_CLT:

```C++
if (emucan.emu_data.cel & emucan.ERR_CLT) {
  Serial.println("WARNING Engine CEL active due to CLT");
}
```

## Sending Data

This can be used to transmit data to the EMU Black, or any other Device on the CAN Bus.

Example: https://github.com/designer2k2/EMUcan/blob/main/examples/EMUcanSendTest/EMUcanSendTest.ino

Define a CAN Message:
```C++
struct can_frame canMsg1;
```

Fill the CAN Message with data:
```C++
canMsg1.can_id  = 0x0F6;
canMsg1.can_dlc = 2;
canMsg1.data[0] = 0xFF;
canMsg1.data[1] = 0x00;
```

Send the CAN Message:
```C++
emucan.sendFrame(&canMsg1);
```

In the EMU Black Software use this to define the receive:
https://github.com/designer2k2/EMUcan/blob/main/examples/EMUcanSendTest/EMUBlackCANStreamExample.canstr


## Custom Frame Receive

Listening to more than just the EMU Black CAN Stream is possible. Every CAN frame can be used.

The advanced example shows it: https://github.com/designer2k2/EMUcan/blob/main/examples/EMUcanAdvancedTest/EMUcanAdvancedTest.ino

in Setup:
```C++
ReturnAllFramesFunction LetMeHaveIt = specialframefunction;
emucan.ReturnAllFrames(LetMeHaveIt);
```

Stop the function:
```C++
emucan.ReturnAllFramesStop();
```

Example receive function:
```C++
void specialframefunction(const struct can_frame *frame) {
  //Magic things can happen here, but dont block!
  Serial.print(frame->can_id, HEX); // print ID
  Serial.print(" ");
  Serial.print(frame->can_dlc, HEX); // print DLC
  Serial.print(" ");

  for (int i = 0; i < frame->can_dlc; i++)  { // print the data
    Serial.print(frame->data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  //Toggle the onboard LED for show:
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
```

# Others

This Library is tested on Arduino Nano with a MCP2515 shield at 8Mhz.

The EMU Black was running Software Version 2.127.

## Different Versions

For Teensy 4 use the EMUcanT4: https://github.com/designer2k2/EMUcanT4

For using the ECUMaster serial stream instead of the CAN Bus use this library: https://github.com/GTO2013/EMUSerial

## Support

Please feel free to use/extend/report bugs/request features!

