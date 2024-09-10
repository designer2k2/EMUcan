#include <SPI.h>
#include <mcp2515.h>

const int SPI_CS_PIN = 10;
MCP2515 mcp2515(SPI_CS_PIN);

// Tablice z przykładowymi wartościami
uint16_t rpms[] = { 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000 };
uint8_t tps[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
int8_t iat[] = { -40, -20, 0, 20, 40, 60, 80, 100, 120 };
uint16_t maps[] = { 0, 100, 200, 300, 400, 500, 600 };
uint16_t pulseWidth[] = { 0, 312, 625, 937, 1250, 1562, 1875, 2187, 2500, 2812 };
float analogIn[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
uint16_t vspd[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };
uint8_t baro[] = { 50, 60, 70, 80, 90, 100, 110, 120, 130 };
uint8_t oilTemp[] = { 0, 20, 40, 60, 80, 100, 120, 140, 160 };
float oilPressure[] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0 };
float fuelPressure[] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0 };
int16_t clt[] = { -40, -20, 0, 20, 40, 60, 80, 100, 120, 130 };
int8_t ignAngle[] = { -60, -30, 0, 30, 60 };
float dwellTime[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
float lambda[] = { 0.6, 0.7, 0.8, 0.9 , 1.0 , 1.1, 1.2};
float lambdaCorrection[] = { 75, 80, 85, 90, 95, 100, 105, 110, 115, 120 };
uint16_t egt1[] = { 0, 200, 400, 600, 800, 1000, 1200, 1400 };
uint16_t egt2[] = { 0, 200, 400, 600, 800, 1000, 1200, 1400 };
uint8_t gear[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
uint8_t emuTemp[] = { 20, 30, 40, 50, 60, 70, 80, 90, 100 };
float batt[] = { 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0 };
uint16_t errFlag[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
uint8_t flags1[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
uint8_t ethanol[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
float dbwPos[] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0 };
float dbwTarget[] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0 };
uint16_t tcDrpmRaw[] = { 0, 100, 200, 300, 400, 500, 600, 700 };
uint16_t tcDrpm[] = { 0, 100, 200, 300, 400, 500, 600, 700 };
uint8_t tcTorqueReduction[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80 };
uint8_t pitLimitTorqueReduction[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80 };
float analogIn5[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
uint8_t outflags[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
uint16_t boostTarget[] = { 0, 100, 200, 300, 400, 500, 600 };
uint8_t pwm1[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };
uint8_t dsgMode[] = { 2, 3, 4, 5, 6, 7, 15 };
float lambdaTarget[] = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 };
uint8_t pwm2[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };
float fuelUsed[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };

void setup() {
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  delay(100);
  Serial.println("------- Emu CAN Simulator ----------");
}

void loop() {
  for (int i = 0; i <= 100; i++) {
    can_frame canMsg1;
    canMsg1.can_id = 0x600;
    canMsg1.can_dlc = 8;

    uint16_t rpm_value = rpms[i % 10];
    uint8_t tps_value = tps[i % 11];
    int8_t iat_value = iat[i % 9];
    uint16_t map_value = maps[i % 7];
    uint16_t pulseWidth_value = pulseWidth[i % 10];
    
    canMsg1.data[0] = rpm_value & 0xFF;
    canMsg1.data[1] = (rpm_value >> 8) & 0xFF;
    canMsg1.data[2] = tps_value;
    canMsg1.data[3] = iat_value;
    canMsg1.data[4] = map_value & 0xFF;
    canMsg1.data[5] = (map_value >> 8) & 0xFF;
    canMsg1.data[6] = pulseWidth_value & 0xFF;
    canMsg1.data[7] = (pulseWidth_value >> 8) & 0xFF;
    mcp2515.sendMessage(&canMsg1);
    delay(50);

    canMsg1.can_id = 0x601;
    canMsg1.data[0] = (uint16_t)(analogIn[i % 10] / 0.0048828125) & 0xFF;
    canMsg1.data[1] = ((uint16_t)(analogIn[i % 10] / 0.0048828125) >> 8) & 0xFF;
    canMsg1.data[2] = (uint16_t)(analogIn[(i + 1) % 10] / 0.0048828125) & 0xFF;
    canMsg1.data[3] = ((uint16_t)(analogIn[(i + 1) % 10] / 0.0048828125) >> 8) & 0xFF;
    canMsg1.data[4] = (uint16_t)(analogIn[(i + 2) % 10] / 0.0048828125) & 0xFF;
    canMsg1.data[5] = ((uint16_t)(analogIn[(i + 2) % 10] / 0.0048828125) >> 8) & 0xFF;
    canMsg1.data[6] = (uint16_t)(analogIn[(i + 3) % 10] / 0.0048828125) & 0xFF;
    canMsg1.data[7] = ((uint16_t)(analogIn[(i + 3) % 10] / 0.0048828125) >> 8) & 0xFF;
    mcp2515.sendMessage(&canMsg1);
    delay(50);

    // Base +2
    canMsg1.can_id = 0x602;
    canMsg1.data[0] = vspd[i % 10] & 0xFF;
    canMsg1.data[1] = (vspd[i % 10] >> 8) & 0xFF;
    canMsg1.data[2] = baro[i % 9];
    canMsg1.data[3] = oilTemp[i % 9];
    canMsg1.data[4] = (uint8_t)(oilPressure[i % 7] / 0.0625);
    canMsg1.data[5] = (uint8_t)(fuelPressure[i % 7] / 0.0625);
    canMsg1.data[6] = clt[i % 10] & 0xFF;
    canMsg1.data[7] = (clt[i % 10] >> 8) & 0xFF;
    mcp2515.sendMessage(&canMsg1);
    delay(50);

    // Base +3
    canMsg1.can_id = 0x603;
    canMsg1.data[0] = ignAngle[i % 5];
    canMsg1.data[1] = (uint8_t)(dwellTime[i % 10] / 0.05);
    canMsg1.data[2] = (uint8_t)(lambda[i % 7] / 0.0078125);
    canMsg1.data[3] = (uint8_t)(lambdaCorrection[i % 10] / 0.5);
    canMsg1.data[4] = egt1[i % 8] & 0xFF;
    canMsg1.data[5] = (egt1[i % 8] >> 8) & 0xFF;
    canMsg1.data[6] = egt2[i % 8] & 0xFF;
    canMsg1.data[7] = (egt2[i % 8] >> 8) & 0xFF;
    mcp2515.sendMessage(&canMsg1);
    delay(50);

    // Base +4
    canMsg1.can_id = 0x604;
    canMsg1.data[0] = gear[i % 9];
    canMsg1.data[1] = emuTemp[i % 9];
    canMsg1.data[2] = (uint16_t)(batt[i % 7] / 0.027) & 0xFF;
    canMsg1.data[3] = ((uint16_t)(batt[i % 7] / 0.027) >> 8) & 0xFF;
    canMsg1.data[4] = errFlag[i % 10] & 0xFF;
    canMsg1.data[5] = (errFlag[i % 10] >> 8) & 0xFF;
    canMsg1.data[6] = flags1[i % 8];
    canMsg1.data[7] = ethanol[i % 11];
    mcp2515.sendMessage(&canMsg1);
    delay(50);

    // Base +5
    canMsg1.can_id = 0x605;
    canMsg1.data[0] = (uint8_t)(dbwPos[i % 9] / 0.5);
    canMsg1.data[1] = (uint8_t)(dbwTarget[i % 9] / 0.5);
    canMsg1.data[2] = tcDrpmRaw[i % 8] & 0xFF;
    canMsg1.data[3] = (tcDrpmRaw[i % 8] >> 8) & 0xFF;
    canMsg1.data[4] = tcDrpm[i % 8] & 0xFF;
    canMsg1.data[5] = (tcDrpm[i % 8] >> 8) & 0xFF;
    canMsg1.data[6] = tcTorqueReduction[i % 9];
    canMsg1.data[7] = pitLimitTorqueReduction[i % 9];
    mcp2515.sendMessage(&canMsg1);
    delay(50);

    // Base +6
    canMsg1.can_id = 0x606;
    canMsg1.data[0] = (uint16_t)(analogIn5[i % 10] / 0.0048828125) & 0xFF;
    canMsg1.data[1] = ((uint16_t)(analogIn5[i % 10] / 0.0048828125) >> 8) & 0xFF;
    canMsg1.data[2] = (uint16_t)(analogIn5[(i + 1) % 10] / 0.0048828125) & 0xFF;
    canMsg1.data[3] = ((uint16_t)(analogIn5[(i + 1) % 10] / 0.0048828125) >> 8) & 0xFF;
    canMsg1.data[4] = outflags[i % 8];
    canMsg1.data[5] = outflags[(i + 1) % 8];
    canMsg1.data[6] = outflags[(i + 2) % 8];
    canMsg1.data[7] = outflags[(i + 3) % 8];
    mcp2515.sendMessage(&canMsg1);
    delay(50);

    // Base +7
    canMsg1.can_id = 0x607;
    canMsg1.data[0] = boostTarget[i % 7] & 0xFF;
    canMsg1.data[1] = (boostTarget[i % 7] >> 8) & 0xFF;
    canMsg1.data[2] = pwm1[i % 10];
    canMsg1.data[3] = dsgMode[i % 7];
    canMsg1.data[4] = (uint8_t)(lambdaTarget[i % 10] / 0.01);
    canMsg1.data[5] = pwm2[i % 10];
    canMsg1.data[6] = (uint16_t)(fuelUsed[i % 8] / 0.01) & 0xFF;
    canMsg1.data[7] = ((uint16_t)(fuelUsed[i % 8] / 0.01) >> 8) & 0xFF;
    mcp2515.sendMessage(&canMsg1);
    delay(300);
  }

  delay(1000);
}
