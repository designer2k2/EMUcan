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

#ifndef _EMUcan_h
#define _EMUcan_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <mcp2515.h>

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

enum EMUcan_STATUS {
  EMUcan_FRESH,
  EMUcan_RECEIVED_WITHIN_LAST_SECOND,
  EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND,
};

class EMUcan {

  public:
    // Constructor
    EMUcan(uint32_t  EMUbase = 0x600, uint8_t cs = 10);

    // Methods
    void begin(const CAN_SPEED canSpeed);
    void begin(const CAN_SPEED canSpeed, const CAN_CLOCK canClock);
    bool checkEMUcan();
    bool sendFrame(const struct can_frame *);

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
      F_SW1  = (1 << 0),
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

    bool decodeCel();
    enum EMUcan_STATUS EMUcan_Status = EMUcan_FRESH;
    MCP2515 *getMcp2515();

    // Privates
  private:

    enum EMU_STATUS_UPDATES {
      EMU_MESSAGE_RECEIVED_VALID,
      EMU_RECEIVED_NOTHING
    };
    uint8_t _cs;
    MCP2515 *mcp2515;
    bool decodeEmuFrame(struct can_frame *msg);
    void emucanstatusEngine(const EMU_STATUS_UPDATES action);
    uint32_t _EMUbase;
    unsigned long _previousMillis = 0;

};
#endif