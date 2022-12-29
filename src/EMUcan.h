/* Copyright (C) designer2k2 Stephan M.
  # This file is part of EMUcanT4 <https://github.com/designer2k2/EMUcanT4>.
  #
  # EMUcanT4 is free software: you can redistribute it and/or modify
  # it under the terms of the GNU General Public License as published by
  # the Free Software Foundation, either version 3 of the License, or
  # (at your option) any later version.
  #
  # EMUcanT4 is distributed in the hope that it will be useful,
  # but WITHOUT ANY WARRANTY; without even the implied warranty of
  # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  # GNU General Public License for more details.
  #
  # You should have received a copy of the GNU General Public License
  # along with EMUcanT4.  If not, see <http://www.gnu.org/licenses/>.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef _EMUcan_h
#define _EMUcan_h

#define EMUCAN_LIB_VERSION (F("2.0.0"))

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

enum EMUcan_STATUS {
  EMUcan_FRESH,
  EMUcan_RECEIVED_WITHIN_LAST_SECOND,
  EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND
};

class EMUcan {

public:
  // Constructor
  EMUcan(const uint32_t EMUbase = 0x600);

  // Methods
  void begin(const uint32_t canSpeed);
  bool checkEMUcan(uint32_t can_id, uint8_t can_dlc, uint8_t data[8]);
  bool decodeCel();
  EMUcan_STATUS EMUcan_Status();

  // Data
  struct emu_data_t emu_data;

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

  static const uint16_t EFLG_ERRORMASK = ERR_CLT
                                         | ERR_IAT
                                         | ERR_MAP
                                         | ERR_WBO
                                         | ERR_EGT1
                                         | ERR_EGT2
                                         | EGT_ALARM
                                         | KNOCKING;

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

  enum OUTFLAGS1 : uint8_t {
    F_PO1 = (1 << 0),
    F_PO2 = (1 << 1),
    F_PO3 = (1 << 2),
    F_PO4 = (1 << 3),
    F_PO5 = (1 << 4),
    F_VPO1 = (1 << 5),
    F_VPO2 = (1 << 6),
    F_VPO3 = (1 << 7)
  };

  enum OUTFLAGS2 : uint8_t {
    F_CANSW1 = (1 << 0),
    F_CANSW2 = (1 << 1),
    F_CANSW3 = (1 << 2),
    F_CANSW4 = (1 << 3),
    F_CANSW5 = (1 << 4),
    F_CANSW6 = (1 << 5),
    F_CANSW7 = (1 << 6),
    F_CANSW8 = (1 << 7)
  };

  enum OUTFLAGS3 : uint8_t {
    F_SW1 = (1 << 0),
    F_SW2 = (1 << 1),
    F_SW3 = (1 << 2),
    F_MUXSW1 = (1 << 3),
    F_MUXSW2 = (1 << 4),
    F_MUXSW3 = (1 << 5),
    F_LC_MAP_SET = (1 << 6),
    F_ALS_MAP_SET = (1 << 7)
  };

  enum OUTFLAGS4 : uint8_t {
    F_FPS = (1 << 0),
    F_CS = (1 << 1),
    F_ACCLUTCH = (1 << 2),
    F_ACFAN = (1 << 3),
    F_NITROUS = (1 << 4),
    F_STARTER_REQ = (1 << 5),
    F_BOOST_MAP_SET = (1 << 6),
  };

  // Privates
private:

  enum EMUcan_STATUS _EMUcan_Status = EMUcan_FRESH;

  enum EMU_STATUS_UPDATES {
    EMU_MESSAGE_RECEIVED_VALID,
    EMU_RECEIVED_NOTHING
  };

  void _decodeEmuFrame(uint32_t can_id, uint8_t can_dlc, uint8_t data[8]);
  void _emucanstatusEngine(const EMU_STATUS_UPDATES action);

  uint32_t _EMUbase;
  unsigned long _previousMillis = 0;
};
#endif
