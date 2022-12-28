#include<unistd.h>
#include<stdexcept>
#include<iostream>

#include "WProgram.h"
#include "EMUcan.h"

using namespace std;

void millis_test() {
  unsigned long start = millis();
  cout << "millis() test start: " << start << endl;
  while( millis() - start < 10000 ) {
    cout << millis() << endl;
    sleep(1);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

void delay_test() {
  unsigned long start = millis();
  cout << "delay() test start: " << start << endl;
  while( millis() - start < 10000 ) {
    cout << millis() << endl;
    delay(250);
  }
  unsigned long end = millis();
  cout << "End of test - duration: " << end - start << "ms" << endl;
}

void run_tests() {
  EMUcan emucan(0x600);
}

int main(int argc, char **argv){
  initialize_mock_arduino();
  // throw std::runtime_error( "Just for the test, let it fail" );
  delay_test();
  millis_test();
  run_tests();
}
