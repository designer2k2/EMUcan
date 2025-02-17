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

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef _EMUcan_h
#define _EMUcan_h

#define EMUCAN_LIB_VERSION (F("2.0.4"))

// Available data
struct emu_data_t {
  uint16_t RPM;                     // RPM
  uint16_t MAP;                     // kPa
  uint8_t TPS;                      // %
  int8_t IAT;                       // C
  float Batt;                       // V
  float IgnAngle;                   // deg
  float pulseWidth;                 // ms
  uint16_t Egt1;                    // C
  uint16_t Egt2;                    // C
  float dwellTime;                  // ms
  int8_t gear;                      //
  uint8_t Baro;                     // kPa
  float analogIn1;                  // V
  float analogIn2;                  // V
  float analogIn3;                  // V
  float analogIn4;                  // V
  float analogIn5;                  // V
  float analogIn6;                  // V
  int8_t emuTemp;                   // C
  float oilPressure;                // Bar
  uint8_t oilTemperature;           // C
  float fuelPressure;               // Bar
  int16_t CLT;                      // C
  float flexFuelEthanolContent;     // %
  float wboLambda;                  // λ
  uint16_t vssSpeed;                // km/h
  float lambdaTarget;               // λ
  uint16_t cel;                     //
  float LambdaCorrection;           // %
  uint8_t flags1;                   // Flags 1
  uint8_t outflags1;                // Outflags 1
  uint8_t outflags2;                // Outflags 2
  uint8_t outflags3;                // Outflags 3
  uint8_t outflags4;                // Outflags 4
  uint8_t pwm1;                     // %
  uint16_t boostTarget;             // kPa
  uint8_t pwm2;                     // %
  float fuel_used;                  // L
  uint8_t DSGmode;                  // DSG mode
  float DBWpos;                     // %
  float DBWtarget;                  // %
  uint16_t TCdrpmRaw;               //
  uint16_t TCdrpm;                  //
  uint8_t TCtorqueReduction;        // %
  uint8_t PitLimitTorqueReduction;  // %
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
  bool checkEMUcan(uint32_t can_id, uint8_t can_dlc, uint8_t data[8]);
  bool decodeCel();
  EMUcan_STATUS EMUcan_Status();

  // Data
  struct emu_data_t emu_data;

  enum ERRORFLAG : uint16_t {
    ERR_CLT = (1 << 0),    // Coolant temperature sensor failed
    ERR_IAT = (1 << 1),    // IAT sensor failed
    ERR_MAP = (1 << 2),    // MAP sensor failed
    ERR_WBO = (1 << 3),    // Wide band oxygen sensor failed
    ERR_EGT1 = (1 << 4),   // EGT sensor #1 failed
    ERR_EGT2 = (1 << 5),   // EGT sensor #2 failed
    EGT_ALARM = (1 << 6),  // EGT too high
    KNOCKING = (1 << 7),   // Knock detected
    FFSENSOR = (1 << 8),   // Flex Fuel sensor failed
    ERR_DBW = (1 << 9),    // Drive by wire failure
    ERR_FPR = (1 << 10)    // Fuel pressure relative error
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
    F_GEARCUT = (1 << 0),          // 1 - Gearcut active
    F_ALS = (1 << 1),              // 1 - ALS active
    F_LC = (1 << 2),               // 1 - Launch control active
    F_IDLE = (1 << 3),             // 1 - Is in idle state
    F_TABLE_SET = (1 << 4),        // 0 - table set 1, 1 - table set 2
    F_TC_INTERVENTION = (1 << 5),  // 1 - traction control intervention
    F_PIT_LIMITER = (1 << 6),      // 1 - Pit limiter active
    F_BRAKE_SWITCH = (1 << 7)      // 1 - Brake switch active
  };

  enum OUTFLAGS1 : uint8_t {
    F_PO1 = (1 << 0),   // Parametric output #1 state
    F_PO2 = (1 << 1),   // Parametric output #2 state
    F_PO3 = (1 << 2),   // Parametric output #3 state
    F_PO4 = (1 << 3),   // Parametric output #4 state
    F_PO5 = (1 << 4),   // Parametric output #5 state
    F_VPO1 = (1 << 5),  // Virtual output #1 state
    F_VPO2 = (1 << 6),  // Virtual output #2 state
    F_VPO3 = (1 << 7)   // Virtual output #3 state
  };

  enum OUTFLAGS2 : uint8_t {
    F_CANSW1 = (1 << 0),  // CAN switch #1 state
    F_CANSW2 = (1 << 1),  // CAN switch #2 state
    F_CANSW3 = (1 << 2),  // CAN switch #3 state
    F_CANSW4 = (1 << 3),  // CAN switch #4 state
    F_CANSW5 = (1 << 4),  // CAN switch #5 state
    F_CANSW6 = (1 << 5),  // CAN switch #6 state
    F_CANSW7 = (1 << 6),  // CAN switch #7 state
    F_CANSW8 = (1 << 7)   // CAN switch #8 state
  };

  enum OUTFLAGS3 : uint8_t {
    F_SW1 = (1 << 0),         // Switch #1 state
    F_SW2 = (1 << 1),         // Switch #2 state
    F_SW3 = (1 << 2),         // Switch #2 state
    F_MUXSW1 = (1 << 3),      // MUX switch #1 state
    F_MUXSW2 = (1 << 4),      // MUX switch #2 state
    F_MUXSW3 = (1 << 5),      // MUX switch #3 state
    F_LC_MAP_SET = (1 << 6),  // Current set of launch control parameters
    F_ALS_MAP_SET = (1 << 7)  // Current set of ALS  parameters
  };

  enum OUTFLAGS4 : uint8_t {
    F_FPS = (1 << 0),           // Fuel pump state
    F_CF = (1 << 1),            // Coolant fan state
    F_ACCLUTCH = (1 << 2),      // AC clutch state
    F_ACFAN = (1 << 3),         // AC fan state
    F_NITROUS = (1 << 4),       // Nitrous active
    F_STARTER_REQ = (1 << 5),   // Starter motor request (from start / stop strategy)
    F_BOOST_MAP_SET = (1 << 6)  // Current set of boost parameters
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
