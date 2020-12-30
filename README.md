# EMUcan
ECUMaster EMU CAN Stream Reader Arduino Library

It reads the EMU CAN Stream and decodes it into something usefull.

It also can send data to the EMU!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/MyLibrary.svg?)](https://www.ardu-badge.com/EMUcan)

## Setup

Install the Library, and the following MCP2515 Library:

MCP2515 Library: https://github.com/autowp/arduino-mcp2515

In the EMU Black, set the CAN-Bus speed to 500 Kpbs and enable "Send EMU stream over CAN-Bus"

The EMU Stream base ID can be changed, the begin function takes this as parameter.

CAN-Bus speed, and MCP2515 Clock speed can be set too, look in the examples.

This Library is tested on Arduino Nano with a MCP2515 shield at 8Mhz.

The EMU Black is running Software Version 2.127.

## Support

Please feel free to use/extend/report bugs/request features!

