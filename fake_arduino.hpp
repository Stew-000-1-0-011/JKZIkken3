#pragma once

#if not defined(ARDUINO)
void pinMode(auto ...){};
void digitalWrite(auto ...){};
void analogWrite(auto ...){};
constexpr auto OUTPUT = 0;
constexpr auto HIGH = 1;
constexpr auto LOW = 0;
#endif