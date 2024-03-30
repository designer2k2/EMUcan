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

 This simulates the full life cycle of the EMUcan library.
 
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

  // Print the library version:
  cout << "EMUcan version: " << EMUCAN_LIB_VERSION << endl;

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
    cout << "EMUcan decode 1st frame ok" << endl;
    cout << "emuTemp: " << std::to_string(emucan.emu_data.emuTemp) << endl;
  } else {
    throw std::runtime_error("EMUcan decode not ok.");
  }

  // Generate another frame:
  uint8_t data2[8] = { 0xf0, 0x02, 0x02, 0x16, 0x25, 0x00, 0x76, 0x00 };
  emucan.checkEMUcan(0x600, 8, data2);

  // Based on the frame from above:
  if (emucan.emu_data.RPM == 752) {
    cout << "EMUcan decode 2nd frame ok" << endl;
    cout << "RPM: " << std::to_string(emucan.emu_data.RPM) << endl;
  } else {
    throw std::runtime_error("EMUcan decode not ok.");
  }

  // Check the flags:
  if (emucan.emu_data.flags1 & emucan.F_IDLE) {
    cout << "EMUcan decode flags ok" << endl;
  } else {
    throw std::runtime_error("EMUcan decode flags not ok.");
  }

  // CEL should not be on:
  if (emucan.decodeCel() == false) {
    cout << "EMUcan CEL check ok" << endl;
  } else {
    throw std::runtime_error("EMUcan CEL check not ok.");
  }

  // Sleep 2 seconds so that the status drops:
  sleep(2);

  // Now the status has to be that something was received:
  if (emucan.EMUcan_Status() == EMUcan_RECEIVED_NOTHING_WITHIN_LAST_SECOND) {
    cout << "EMUcan status update ok" << endl;
  } else {
    throw std::runtime_error("EMUcan status update not ok.");
  }

  // Enable GPS:
  emucan.enableGPS(0x400);

  // Generate GPS frame:
  uint8_t data3[8] = { 0x1d, 0xd9, 0x43, 0xb0, 0x0b, 0xdc, 0xa0, 0x42 };
  emucan.checkEMUcan(0x400, 8, data3);

  // Now the status has to be that something was received:
  if (emucan.EMUcan_GPS_Status() == EMUcan_RECEIVED_WITHIN_LAST_SECOND) {
    cout << "EMUcan GPS status update ok" << endl;
  } else {
    throw std::runtime_error("EMUcan GPS status update not ok.");
  }

  // Based on the frame from above:
  if (emucan.emu_data_gps.Latitude == 752) {
    cout << "EMUcan decode 3nd frame ok" << endl;
    cout << "Latitude: " << std::to_string(emu_data_gps.Latitude) << endl;
  } else {
    cout << "Latitude: " << std::to_string(emu_data_gps.Latitude) << endl;
    throw std::runtime_error("EMUcan decode not ok.");
  }

  cout << "EMUcan check complete, all ok." << endl;
}

int main() {
  initialize_mock_arduino();
  run_tests();
}
