#pragma once

#if not defined(__AVR)
void pinMode(int, int){}
void digitalWrite(int, int){}
void analogWrite(int, int){}
void micros(int){}
void delayMicroseconds(int){}
constexpr auto OUTPUT = 0;
constexpr auto HIGH = 1;
constexpr auto LOW = 0;
#endif