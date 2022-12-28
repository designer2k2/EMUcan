#include <sys/timeb.h>
#include <stdint.h>
#include "WProgram.h"

timeb t_start;
unsigned long millis() {
  timeb t_now;
  ftime(&t_now);
  return (t_now.time  - t_start.time) * 1000 + (t_now.millitm - t_start.millitm);
}

void delay(unsigned long ms) {
  unsigned long start = millis();
  while(millis() - start < ms){}
}

void initialize_mock_arduino() {
  ftime(&t_start);
}
