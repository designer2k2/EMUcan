#include <stdint.h>

#pragma once

typedef unsigned char byte;
typedef unsigned short int word;

unsigned long millis();
void delay(unsigned long ms);
unsigned long millis();

void initialize_mock_arduino(); 
