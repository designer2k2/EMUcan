#include <time.h>
#include "mock_arduino.h"

struct timespec t_start;
unsigned long millis() {
  struct timespec t_now;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t_now);
  return (t_now.tv_sec - t_start.tv_sec) * 1000 + (t_now.tv_nsec - t_start.tv_nsec) / 1000000;
}

void delay(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {}
}

void initialize_mock_arduino() {
  clock_gettime(CLOCK_MONOTONIC_RAW, &t_start);
}
