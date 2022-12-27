// EMUCan Lib Simple Test Example

// Example to be run on ESP32
// Configure the EMU Black to send the CAN Stream at 500KBPS

// https://www.designer2k2.at
// Stephan Martin 27.12.2022


#include "EMUcan.h"
EMUcan emucan(0x600);

#include "driver/twai.h"

// Pins used to connect to CAN bus transceiver:
#define RX_PIN 21
#define TX_PIN 22

unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  Serial.println("------- CAN Read ----------");

  // Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_LISTEN_ONLY);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  //Look in the api-reference for other speed sets.
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("Driver installed");
  } else {
    Serial.println("Failed to install driver");
    return;
  }

  // Start TWAI driver
  if (twai_start() == ESP_OK) {
    Serial.println("Driver started");
  } else {
    Serial.println("Failed to start driver");
    return;
  }

  // Reconfigure alerts to detect frame receive, Bus-Off error and RX queue full states
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_QUEUE_FULL;
  if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
    Serial.println("CAN Alerts reconfigured");
  } else {
    Serial.println("Failed to reconfigure alerts");
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // Call the EMUcan lib with every received frame:
  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(1));
  if (alerts_triggered & TWAI_ALERT_RX_DATA) {
    // One or more messages received. Handle all.
    twai_message_t message;
    while (twai_receive(&message, 0) == ESP_OK) {
      emucan.checkEMUcan(message.identifier, message.data_length_code, message.data);
    }
  }

  // Serial out every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (emucan.EMUcan_Status == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
      Serial.print(emucan.emu_data.RPM);
      Serial.print(";");
      Serial.print(emucan.emu_data.TPS);
      Serial.print(";");
      Serial.print(emucan.emu_data.IAT);
      Serial.print(";");
      Serial.print(emucan.emu_data.MAP);
      Serial.print(";");
      Serial.println(emucan.emu_data.pulseWidth);
    } else {
      Serial.print(emucan.emu_data.RPM);
      Serial.println("No communication from EMU");
    }
    if (emucan.emu_data.flags1 & emucan.F_IDLE) {
      Serial.println("Engine Idle active");
    }
  }
}
