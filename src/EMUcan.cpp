/* Copyright (C) designer2k2 Stephan M.
  # This file is part of EMUcan <https://github.com/designer2k2/EMUcan>.
  #
  # EMUcan is free software: you can redistribute it and/or modify
  # it under the terms of the GNU General Public License as published by
  # the Free Software Foundation, either version 3 of the License, or
  # (at your option) any later version.
  #
  # EMUcan is distributed in the hope that it will be useful,
  # but WITHOUT ANY WARRANTY; without even the implied warranty of
  # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  # GNU General Public License for more details.
  #
  # You should have received a copy of the GNU General Public License
  # along with EMUcan.  If not, see <http://www.gnu.org/licenses/>.
*/

// Look up the Emu Can Stream documentation in your local installed ECUMASTER Emu Black Software:
// file:///C:/Program%20Files%20(x86)/Ecumaster/EMU%20Black/Help/EN/emuCANStream.html

#include "EMUcan.h"
#include <mcp2515.h>

struct can_frame canMsg;

EMUcan::EMUcan(uint32_t EMUbase, uint8_t cs) {
  //Getting the base number, as set in the EMU Software
  _EMUbase = EMUbase;
  _cs = cs;
}

void EMUcan::begin(const CAN_SPEED canSpeed) {
  begin(canSpeed, MCP_16MHZ);
}

void EMUcan::begin(const CAN_SPEED canSpeed, CAN_CLOCK canClock) {
  mcp2515 = new MCP2515(_cs);
  mcp2515->reset();
  mcp2515->setBitrate(canSpeed, canClock);
  mcp2515->setNormalMode();
}

bool EMUcan::checkEMUcan() {
  if (mcp2515->readMessage(&canMsg) == MCP2515::ERROR_OK) {
    //Check if Message is within Range of 0-7 from base:
    if ( canMsg.can_id >= _EMUbase && canMsg.can_id <= _EMUbase + 7) {
      //So messages here should be decoded!
      decodeEmuFrame(&canMsg);
      //Store the event:
      emucanstatusEngine(EMU_MESSAGE_RECEIVED_VALID);
    }
  } else {
    emucanstatusEngine(EMU_RECEIVED_NOTHING);
  }
  return true;
}

void EMUcan::emucanstatusEngine(const EMU_STATUS_UPDATES action) {
  //check the current time versus the last to define the status.
  unsigned long currentMillis = millis();
  switch (action) {
    case EMU_RECEIVED_NOTHING:
      if (currentMillis - _previousMillis >= 1000) {
        EMUcan_Status = EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND;
      }
      break;
    case EMU_MESSAGE_RECEIVED_VALID:
      _previousMillis = currentMillis;
      EMUcan_Status = EMUcan_RECEIVED_WITHIN_LAST_SECOND;
      break;
    default:
      break;
  }
}

bool EMUcan::sendFrame(const struct can_frame *frame) {
  if (mcp2515->sendMessage(frame) != MCP2515::ERROR_OK) {
    return false;
  }
  return true;
}

bool EMUcan::decodeEmuFrame(struct can_frame *msg) {
  //This decodes the frames and fills them into the data:
  //Base:
  if (msg->can_id == _EMUbase) {
    //0-1 RPM in 16Bit unsigned
    emu_data.RPM = (msg->data[1] << 8) + msg->data[0];
    //2 TPS in /2 %
    emu_data.TPS = msg->data[2] / 2;
    //3 IAT 8bit signed -40-127°C
    emu_data.IAT = msg->data[3];
    //4-5 MAP 16Bit 0-600kpa
    emu_data.MAP = (msg->data[5] << 8) + msg->data[4];
    //6-7 INJPW 0-50 0.016129ms
    emu_data.pulseWidth = ((msg->data[7] << 8) + msg->data[6]) * 0.016129;
  }
  //Base +1:
  if (msg->can_id == _EMUbase + 1) {
    //AIN in 16Bit unsigned  0.0048828125 V/bit
    emu_data.analogIn1 = ((msg->data[1] << 8) + msg->data[0]) * 0.0048828125;
    emu_data.analogIn2 = ((msg->data[3] << 8) + msg->data[2]) * 0.0048828125;
    emu_data.analogIn3 = ((msg->data[5] << 8) + msg->data[4]) * 0.0048828125;
    emu_data.analogIn4 = ((msg->data[7] << 8) + msg->data[6]) * 0.0048828125;
  }
  //Base +2:
  if (msg->can_id == _EMUbase + 2) {
    //0-1 VSPD in 16Bit unsigned
    emu_data.vssSpeed = (msg->data[1] << 8) + msg->data[0];
    //2 BARO kPa
    emu_data.Baro = msg->data[2];
    //3 OILT 0-160°C
    emu_data.oilTemperature = msg->data[3];
    //4 OILP BAR 0.0625
    emu_data.oilPressure = msg->data[4] * 0.0625;
    //5 FUELP BAR 0.03125
    emu_data.fuelPressure = msg->data[5] * 0.03125;
    //6-7 CLT 16bit Signed 0.016129ms
    emu_data.CLT = ((msg->data[7] << 8) + msg->data[6]);
  }
  //Base +3:
  if (msg->can_id == _EMUbase + 3) {
    //0 IGNANG in 8Bit signed    -60 60  0.5deg/bit
    emu_data.IgnAngle = msg->data[0] / 2;
    //1 DWELL 0-10ms 0.05ms/bit
    emu_data.dwellTime = msg->data[1] * 0.05;
    //2 LAMBDA 8bit 0-2 0.0078125 L/bit
    emu_data.wboLambda = msg->data[2] * 0.0078125;
    //3 LAMBDACORR 75-125 0.5%
    emu_data.LambdaCorrection = msg->data[3] / 2;
    //4-5 EGT1 16bit °C
    emu_data.Egt1 = ((msg->data[5] << 8) + msg->data[4]);
    //6-7 EGT2 16bit °C
    emu_data.Egt2 = ((msg->data[7] << 8) + msg->data[6]);
  }
  //Base +4:
  if (msg->can_id == _EMUbase + 4) {
    //0 GEAR
    emu_data.gear = msg->data[0];
    //1 ECUTEMP °C
    emu_data.emuTemp = msg->data[1];
    //2-3 BATT 16bit  0.027 V/bit
    emu_data.Batt = ((msg->data[3] << 5) + msg->data[2]) * 0.027;
    //4-5 ERRFLAG 16bit
    emu_data.cel = ((msg->data[5] << 8) + msg->data[4]);
    //6 FLAGS1 8bit
    emu_data.flags1 = msg->data[6];
    //7 ETHANOL %
    emu_data.flexFuelEthanolContent = msg->data[7];
  }
  //Base +6:
  if (msg->can_id == _EMUbase + 6) {
    //AIN in 16Bit unsigned  0.0048828125 V/bit
    emu_data.analogIn5 = ((msg->data[1] << 8) + msg->data[0]) * 0.0048828125;
    emu_data.analogIn6 = ((msg->data[3] << 8) + msg->data[2]) * 0.0048828125;
    emu_data.outflags1 = msg->data[4];
    emu_data.outflags2 = msg->data[5];
    emu_data.outflags3 = msg->data[6];
    emu_data.outflags4 = msg->data[7];
  }
  //Base +7:
  if (msg->can_id == _EMUbase + 7) {
    //Boost target 16bit 0-600 kPa
    emu_data.boostTarget = ((msg->data[1] << 8) + msg->data[0]);
    //PWM#1 DC 1%/bit
    emu_data.pwm1 = msg->data[2];
  }
  return true;
}

MCP2515 *EMUcan::getMcp2515() {
  return mcp2515;
}

bool EMUcan::decodeCel() {
  //Returns true if an CEL error is on:
  if ( emu_data.cel & EFLG_ERRORMASK ) {
    return true;
  } else {
    return false;
  }
}