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


EMUcan::EMUcan(const uint32_t EMUbase) {
  //Getting the base number, as set in the EMU Software
  _EMUbase = EMUbase;
}

bool EMUcan::checkEMUcan(uint32_t can_id, uint8_t can_dlc, uint8_t data[8]) {
  //Check if Message is within Range of 0-7 from base:
  if (can_id >= _EMUbase && can_id <= _EMUbase + 7) {
    //So messages here should be decoded!
    decodeEmuFrame(can_id, can_dlc, data);
    //Store the event:
    emucanstatusEngine(EMU_MESSAGE_RECEIVED_VALID);
    return true;
  } else {
    emucanstatusEngine(EMU_RECEIVED_NOTHING);
    return false;
  }
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

void EMUcan::decodeEmuFrame(uint32_t can_id, uint8_t can_dlc, uint8_t data[8]) {
  //This decodes the frames and fills them into the data:

  //Base:
  if (can_id == _EMUbase) {
    //0-1 RPM in 16Bit unsigned
    emu_data.RPM = (data[1] << 8) + data[0];
    //2 TPS in /2 %
    emu_data.TPS = data[2] * 0.5;
    //3 IAT 8bit signed -40-127°C
    emu_data.IAT = int8_t(data[3]);
    //4-5 MAP 16Bit 0-600kpa
    emu_data.MAP = (data[5] << 8) + data[4];
    //6-7 INJPW 0-50 0.016129ms
    emu_data.pulseWidth = ((data[7] << 8) + data[6]) * 0.016129;
  }
  //Base +1:
  if (can_id == _EMUbase + 1) {
    //AIN in 16Bit unsigned  0.0048828125 V/bit
    emu_data.analogIn1 = ((data[1] << 8) + data[0]) * 0.0048828125;
    emu_data.analogIn2 = ((data[3] << 8) + data[2]) * 0.0048828125;
    emu_data.analogIn3 = ((data[5] << 8) + data[4]) * 0.0048828125;
    emu_data.analogIn4 = ((data[7] << 8) + data[6]) * 0.0048828125;
  }
  //Base +2:
  if (can_id == _EMUbase + 2) {
    //0-1 VSPD in 16Bit unsigned 1 kmh/h / bit
    emu_data.vssSpeed = (data[1] << 8) + data[0];
    //2 BARO 50-130 kPa
    emu_data.Baro = data[2];
    //3 OILT 0-160°C
    emu_data.oilTemperature = data[3];
    //4 OILP BAR 0.0625 bar/bit
    emu_data.oilPressure = data[4] * 0.0625;
    //5 FUELP BAR 0.0625 bar/bit
    emu_data.fuelPressure = data[5] * 0.0625;
    //6-7 CLT 16bit Signed -40-250 1 C/bit
    emu_data.CLT = int16_t(((data[7] << 8) + data[6]));
  }
  //Base +3:
  if (can_id == _EMUbase + 3) {
    //0 IGNANG in 8Bit signed    -60 60  0.5deg/bit
    emu_data.IgnAngle = int8_t(data[0]) * 0.5;
    //1 DWELL 0-10ms 0.05ms/bit
    emu_data.dwellTime = data[1] * 0.05;
    //2 LAMBDA 8bit 0-2 0.0078125 L/bit
    emu_data.wboLambda = data[2] * 0.0078125;
    //3 LAMBDACORR 75-125 0.5%
    emu_data.LambdaCorrection = data[3] * 0.5;
    //4-5 EGT1 16bit °C
    emu_data.Egt1 = ((data[5] << 8) + data[4]);
    //6-7 EGT2 16bit °C
    emu_data.Egt2 = ((data[7] << 8) + data[6]);
  }
  //Base +4:
  if (can_id == _EMUbase + 4) {
    //0 GEAR
    emu_data.gear = data[0];
    //1 ECUTEMP °C
    emu_data.emuTemp = data[1];
    //2-3 BATT 16bit  0.027 V/bit
    emu_data.Batt = ((data[3] << 8) + data[2]) * 0.027;
    //4-5 ERRFLAG 16bit
    emu_data.cel = ((data[5] << 8) + data[4]);
    //6 FLAGS1 8bit
    emu_data.flags1 = data[6];
    //7 ETHANOL %
    emu_data.flexFuelEthanolContent = data[7];
  }
  //Base +5:
  if (can_id == _EMUbase + 5) {
    //0 DBW Pos 0.5%
    emu_data.DBWpos = data[0] * 0.5;
    //1 DBW Target 0.5%
    emu_data.DBWtarget = data[1] * 0.5;
    //2-3 TC DRPM RAW 16bit  1/bit
    emu_data.TCdrpmRaw = ((data[3] << 8) + data[2]);
    //4-5 TC DRPM 16bit  1/bit
    emu_data.TCdrpm = ((data[5] << 8) + data[4]);
    //6 TC Torque reduction %
    emu_data.TCtorqueReduction = data[6];
    //7 Pit Limit Torque reduction %
    emu_data.PitLimitTorqueReduction = data[7];
  }
  //Base +6:
  if (can_id == _EMUbase + 6) {
    //AIN in 16Bit unsigned  0.0048828125 V/bit
    emu_data.analogIn5 = ((data[1] << 8) + data[0]) * 0.0048828125;
    emu_data.analogIn6 = ((data[3] << 8) + data[2]) * 0.0048828125;
    emu_data.outflags1 = data[4];
    emu_data.outflags2 = data[5];
    emu_data.outflags3 = data[6];
    emu_data.outflags4 = data[7];
  }
  //Base +7:
  if (can_id == _EMUbase + 7) {
    //0-1 Boost target 16bit 0-600 kPa
    emu_data.boostTarget = ((data[1] << 8) + data[0]);
    //2 PWM#1 DC 1%/bit
    emu_data.pwm1 = data[2];
    //3 DSG mode 2=P 3=R 4=N 5=D 6=S 7=M 15=fault
    emu_data.DSGmode = data[3];
    //since version 143 this contains more data, check lenght:
    if (can_dlc == 8) {
      //4 Lambda target 8bit 0.01%/bit
      emu_data.lambdaTarget = data[4] * 0.01;
      //5 PWM#2 DC 1%/bit
      emu_data.pwm2 = data[5];
      //6-7 Fuel used 16bit 0.01L/bit
      emu_data.fuel_used = ((data[7] << 8) + data[6]) * 0.01;
    }
  }
}

bool EMUcan::decodeCel() {
  //Returns true if an CEL error is on:
  if (emu_data.cel & EFLG_ERRORMASK) {
    return true;
  } else {
    return false;
  }
}
