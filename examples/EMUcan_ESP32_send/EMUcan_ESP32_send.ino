// EMUCan Lib Test Example with sending to the EMU

// Example to be run on ESP32
// Configure the EMU Black to send the CAN Stream at 500KBPS

// Attach Button or Switch between BUTTON1_PIN/BUTTON2_PIN and ground.

// Configure custom CAN messages in the EMU Black:
// Use this file: EMUBlackCANStreamExample.canstr
// It will receive the values as CAN switch 1 and 2

// https://www.designer2k2.at
// Stephan Martin 02.04.2025


#define BUTTON1_PIN 32
#define BUTTON2_PIN 33

#include "EMUcan.h"
// EMU initialized with base ID 600:
EMUcan emucan(0x600);

// Needed for the CAN Interface on the ESP32 (called TWAI):
#include "driver/twai.h"

// Pins used to connect to CAN bus transceiver:
#define RX_PIN 21
#define TX_PIN 22

#define PRINT_RATE_MS 500
#define TRANSMIT_RATE_MS 1000
unsigned long previousMillis = 0;
unsigned long previousMillis_tx = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  Serial.println("------- CAN Read ----------");

  // Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  //Look in the api-reference for other speed sets.
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("Driver installed");
  } else {
    Serial.println("Failed to install driver");
  }

  // Start TWAI driver
  if (twai_start() == ESP_OK) {
    Serial.println("Driver started");
  } else {
    Serial.println("Failed to start driver");
  }

  // Reconfigure alerts to detect TX alerts and Bus-Off errors and RX queue full states
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS | TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR;
  if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
    Serial.println("CAN Alerts reconfigured");
  } else {
    Serial.println("Failed to reconfigure alerts");
    return;
  }

  // Define Button inputs with pullup:
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

// CAN Bus send Switch States:
void Send_CAN_Switch_States() {
  if (emucan.EMUcan_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
    byte CAN_Switch_State = 0;

    bool CAN_Switch_1 = digitalRead(BUTTON1_PIN);
    bool CAN_Switch_2 = digitalRead(BUTTON2_PIN);

    bitWrite(CAN_Switch_State, 0, CAN_Switch_1);
    bitWrite(CAN_Switch_State, 1, CAN_Switch_2);

    Serial.print("Send Switch State over CAN: ");
    Serial.println(CAN_Switch_State, BIN);

    // Configure message to transmit
    twai_message_t message;
    message.identifier = 0x0F7;
    message.data_length_code = 1;
    message.data[0] = CAN_Switch_State;
    //Sends the frame;
    // Queue message for transmission
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
      Serial.println("Message queued for transmission");
    } else {
      Serial.println("Failed to queue message for transmission");
    }
  } else {
    Serial.println("Did not send Switch State over CAN due to no communication.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // Call the EMUcan lib with every received frame:
  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(1));
  twai_status_info_t twaistatus;
  twai_get_status_info(&twaistatus);
  if (alerts_triggered & TWAI_ALERT_RX_DATA) {
    // One or more messages received. Handle all.
    twai_message_t message;
    while (twai_receive(&message, 0) == ESP_OK) {
      emucan.checkEMUcan(message.identifier, message.data_length_code, message.data);
    }
  }

  // Handle alerts
  if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
    Serial.println("Alert: TWAI controller has become error passive.");
  }
  if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
    Serial.println("Alert: A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus.");
    Serial.printf("Bus error count: %lu\n", twaistatus.bus_error_count);
  }
  if (alerts_triggered & TWAI_ALERT_TX_FAILED) {
    Serial.println("Alert: The Transmission failed.");
    Serial.printf("TX buffered: %lu\t", twaistatus.msgs_to_tx);
    Serial.printf("TX error: %lu\t", twaistatus.tx_error_counter);
    Serial.printf("TX failed: %lu\n", twaistatus.tx_failed_count);
  }
  if (alerts_triggered & TWAI_ALERT_TX_SUCCESS) {
    Serial.println("Alert: The Transmission was successful.");
    Serial.printf("TX buffered: %lu\t", twaistatus.msgs_to_tx);
  }

  // Send message every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis_tx >= TRANSMIT_RATE_MS) {
    previousMillis_tx = currentMillis;
    Send_CAN_Switch_States();
  }

  // Serial out Status every second:
  if (currentMillis - previousMillis >= PRINT_RATE_MS) {
    previousMillis = currentMillis;
    if (emucan.EMUcan_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
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
      Serial.println("No communication from EMU");
    }
    if (emucan.emu_data.flags1 & emucan.F_IDLE) {
      Serial.println("Engine Idle active");
    }
    if (emucan.decodeCel()) {
      Serial.println("WARNING Engine CEL active");
    }
  }
}
