#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <string>

#include "WProgram.h"
#include "EMUcan.h"

using namespace std;

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
  if (emucan.emu_data.emuTemp == 15) {
    cout << "EMUcan decode frame ok" << endl;
    cout << "emuTemp: " << std::to_string(emucan.emu_data.emuTemp) << endl;
  } else {
    throw std::runtime_error("EMUcan decode not ok.");
  }

  // Sleep 2 seconds that the status drops:
  sleep(2);

  // Now the status has to be that something was received:
  if (emucan.EMUcan_Status() == EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND) {
    cout << "EMUcan status update ok" << endl;
  } else {
    throw std::runtime_error("EMUcan status update not ok.");
  }

  cout << "EMUcan check complete, all ok." << endl;
}

int main() {
  initialize_mock_arduino();
  run_tests();
}
