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

class EMUcan {

  public:
    // Constructor
    EMUcan(uint32_t  EMUbase=0x600, byte cantype=1);
    
    // Methods
    void begin();
    bool checkEMUcan();
    bool sendFrame();

    // Data
    struct emu_data_t emu_data;
    struct can_frame send_frame;

    // Privates
  private:
    bool decodeEmuFrame(struct can_frame *msg);
    bool decodeCel();
    uint32_t _EMUbase;
    byte _cantype;
};
#endif
