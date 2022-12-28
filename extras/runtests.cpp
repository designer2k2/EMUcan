#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <string>

#include "WProgram.h"
#include "EMUcan.h"

using namespace std;

void millis_test() {
  unsigned long start = millis();
  cout << "millis() test start: " << start << endl;
  while (millis() - start < 10000) {
    cout << millis() << endl;
    sleep(1);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

void delay_test() {
  unsigned long start = millis();
  cout << "delay() test start: " << start << endl;
  while (millis() - start < 10000) {
    cout << millis() << endl;
    delay(250);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

void run_tests() {
  // Init the library:
  EMUcan emucan(0x600);

  // Check if the init was ok, status has to be fresh:
  if (emucan.EMUcan_Status() == EMUcan_FRESH) {
    cout << "EMUcan init ok" << endl;
  } else {
    throw std::runtime_error("EMUcan init not ok");
  }

  // Generate a can frame and hand it over:
  uint8_t data[8] = { 0x00, 0x0f, 0x13, 0x02, 0x00, 0x00, 0x08, 0x00 };
  emucan.checkEMUcan(0x604, 8, data);

  // Now the status has to be that something was received:
  if (emucan.EMUcan_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
    cout << "EMUcan status update ok" << endl;
  } else {
    throw std::runtime_error("EMUcan status update not ok.");
  }

  // Based on the frame from above:
  cout << "emuTemp: " << std::to_string(emucan.emu_data.emuTemp) << endl;
}

int main(int argc, char **argv) {
  initialize_mock_arduino();
  //delay_test();
  //millis_test();
  run_tests();
}
