# EMUcan
ECUMaster EMU CAN Stream Reader Arduino Library

It reads the EMU CAN Stream and decodes it into something usefull.

It also can send data to the EMU!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/EMUcan.svg?)](https://www.ardu-badge.com/EMUcan)

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

In the EMU Black, set the CAN-Bus speed to 500 Kpbs and enable "Send EMU stream over CAN-Bus"

The EMU Stream base ID can be changed, the begin function takes this as parameter.

CAN-Bus speed, and MCP2515 Clock speed can be set too, look in the examples.

This Library is tested on Arduino Nano with a MCP2515 shield at 8Mhz.

The EMU Black is running Software Version 2.127.

## Other Versions

For Teensy 4 use the EMUcanT4: https://github.com/designer2k2/EMUcanT4

## Support

Please feel free to use/extend/report bugs/request features!

