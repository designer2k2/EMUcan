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
  
 To be able to test the EMUcan it has to run inside a Github Action.
 This simulates needed parts from the Arduino environment to make it happen.

 Based on the work from Erik Elmore: 
 https://stackoverflow.com/questions/780819/how-can-i-unit-test-arduino-code
*/

#include <stdint.h>

#pragma once

unsigned long millis();

void initialize_mock_arduino();
