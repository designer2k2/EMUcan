# EMUcan
ECUMaster EMU CAN Stream Reader Library

It reads the EMU CAN Stream and decodes it into something usefull.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![arduino-library-badge](https://www.ardu-badge.com/badge/MyLibrary.svg?)](https://www.ardu-badge.com/EMUcan)

## Setup

This Library is tested on Arduino Nano with a MCP2515 shield at 8Mhz.
The EMU Black is running Software Version 2.127.

Use this MCP2515 Library: https://github.com/autowp/arduino-mcp2515

In the EMU Black, set the CAN-Bus speed to 500 Kpbs and enable "Send EMU stream over CAN-Bus"

The EMU Stream base ID can be changed, the begin function takes this as parameter.

If you want to change the CAN-Bus speed, or the 8Mhz Clock from the MCP2515, you currently need to do this in the Library directly.

## Support

Please feel free to use/extend/report bugs/request features!

