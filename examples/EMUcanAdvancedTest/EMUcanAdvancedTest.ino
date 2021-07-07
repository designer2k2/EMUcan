// EMUCan Lib Advanced Test Example

// Example to be run on Arduino Nano with MCP2515
// Configure the EMU Black to send the CAN Stream at 500KBPS

// This MCP2515 Lib is used:
// https://github.com/autowp/arduino-mcp2515


// https://www.designer2k2.at
// Stephan Martin 28.12.2020

#include <EMUcan.h>

//Define the EMU Can Base at 600, and the CS Pin from the MCP215 at 10):
EMUcan emucan(0x600, 10);

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  //Call this in the setup to init the lib:
  emucan.begin(CAN_500KBPS, MCP_8MHZ);

  Serial.print("EMUCAN_LIB_VERSION: ");
  Serial.println(EMUCAN_LIB_VERSION);

  //Setup the Callback to receive every CAN Message:
  ReturnAllFramesFunction LetMeHaveIt = specialframefunction;
  emucan.ReturnAllFrames(LetMeHaveIt);

  Serial.println("------- CAN Read ----------");

}

void loop() {
  // This function should be called as often as possible
  emucan.checkEMUcan();

  // only send every second:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (emucan.EMUcan_Status == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
      Serial.println(emucan.emu_data.RPM);
      // Check the FLAGS1 if the Engine is Idle:
      if (emucan.emu_data.flags1 & emucan.F_IDLE) {
        Serial.println("Engine Idle active");
      }
    } else {
      Serial.println("No communication from EMU");
    }
    //Stop sending all frames after 1 second, this spams the serial
    emucan.ReturnAllFramesStop();

    //Check RX/TX CAN error counts:
    Serial.print("RX error count: ");
    Serial.println(emucan.CanErrorCounter(false));
    Serial.print("TX error count: ");
    Serial.println(emucan.CanErrorCounter(true));

    //retreive the mcp2515 object for direct access
    MCP2515 mcp = *emucan.getMcp2515();
    //call the getErrorFlags function from the mcp2515 lib:
    uint8_t eflg = mcp.getErrorFlags();
    Serial.print("eflg register:");
    Serial.println(eflg);
  }

}

// self defined function to handle all frames:
void specialframefunction(const struct can_frame *frame) {
  //Magic things can happen here, but dont block!
  Serial.print(frame->can_id, HEX); // print ID
  Serial.print(" ");
  Serial.print(frame->can_dlc, HEX); // print DLC
  Serial.print(" ");

  for (int i = 0; i < frame->can_dlc; i++)  { // print the data
    Serial.print(frame->data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  //Toggle the onboard LED for show:
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
