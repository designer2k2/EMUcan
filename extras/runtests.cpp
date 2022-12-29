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

 This simulates the full live cycle of the EMUcan library.
 
 Based on the work from Erik Elmore: 
 https://stackoverflow.com/questions/780819/how-can-i-unit-test-arduino-code
*/

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
