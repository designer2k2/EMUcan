# EMUcan
ECUMaster EMU CAN Stream Reader Arduino Library

It reads the EMU CAN Stream and decodes it into something usefull.

It also can send data to the EMU!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/EMUcan.svg?)](https://www.ardu-badge.com/EMUcan)

**Content:**

(ToDo ToC)

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

This Library is tested on Arduino Nano with a MCP2515 shield at 8Mhz.

The EMU Black is running Software Version 2.127.

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

(ToDo)

## Reading Flags

(ToDo)

## Sending Data

(ToDo)

## Custom Frame Receive

(ToDo)

# Others

## Different Versions

For Teensy 4 use the EMUcanT4: https://github.com/designer2k2/EMUcanT4

## Support

Please feel free to use/extend/report bugs/request features!

