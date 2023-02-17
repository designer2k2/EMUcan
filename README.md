# EMUcan
ECUMaster EMU CAN Stream Reader Arduino Library

It reads the EMU CAN Stream and decodes it into something useful.

This works with any CAN enabled device, MCP2515, Teensy, ESP32, STM32 and more.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/EMUcan.svg?)](https://www.ardu-badge.com/EMUcan) [![CI badge](https://github.com/designer2k2/EMUcan/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/designer2k2/EMUcan/actions) [![CI badge](https://github.com/designer2k2/EMUcan/actions/workflows/unit-test.yml/badge.svg)](https://github.com/designer2k2/EMUcan/actions)

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
* [Others](#others)
   * [Different Versions](#different-versions)
   * [Support](#support)
   * [Migrate](#migrate-from-version-below-2-or-the-emucant4)

## Installation

To install EMUcan into your Arduino IDE you can use the Library Manager (available from Arduino IDE version 1.6.2). Open the IDE and click to the `Sketch` menu and then `Include Library` → `Manage Libraries`. There search for "EMUcan" and click `Install`.

Or alternatively install it as ZIP file:

[Download](https://github.com/designer2k2/EMUcan/archive/main.zip) the zip file from our [repository](https://github.com/designer2k2/EMUcan/) and select in Arduino IDE `Sketch` → `Include library` → `Add .ZIP library...`.

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

For the MCP2515: https://github.com/autowp/arduino-mcp2515#can-shield

For ESP32 / Teensy: Tested CAN Bus Transceiver can be found here: https://github.com/PaulStoffregen/FlexCAN

# Software usage

## Initialization

To start the library with EMU Can Base (600 by default)

```C++
EMUcan emucan(0x600);
```

You need to set up the receiving of CAN frames, see in the examples on how to to that. And then hand over the important parts to the EMUcan library.

## Check on CAN Bus updates

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

## Status

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


# Others

This Library is tested on Arduino Nano with a MCP2515 shield at 8Mhz.

Further on a Teensy4, Teensy3 and ESP32 with SN65HVD232 transceiver.

The EMU Black was running Software Version 2.154.

## Different Versions

For using the ECUMaster serial stream instead of the CAN Bus use this library: https://github.com/GTO2013/EMUSerial

## Migrate from version below 2, or the EMUcanT4

before Version 2, or in the EMUcanT4, the CAN Bus handling was part of the EMUcan lib. Now from Version 2 onwards you have to basically set this up by yourself and handover the CAN frame into the EMUcan library. 

This gives you full control over the CAN interface, it also enables this library to be run on basically every hardware.

Please see in the examples on how to make this happen on MCP2515, Teensy and ESP32.

## Support

Please feel free to use/extend/report bugs/request features!
