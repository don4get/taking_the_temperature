// Dummy implementation of the tmod.
// The implement is kept to strict minimum simplicity, just used for linking
// tests.

#ifndef LIBTMOD_LIBRARY_H
#define LIBTMOD_LIBRARY_H

#include <cstdint>

constexpr uint8_t TMOD_MAX_ADCS = 14;
constexpr int16_t TMOD_DEFAULT_ADC_VALUE = 4;
constexpr int16_t TMOD_INVALID_VOLTAGE_MEASUREMENT = -1;
constexpr uint16_t TMOD_MAX_ADC_VALUE = 16383;

int16_t tmodReadAdc(uint16_t hardwareAddress);

uint16_t tmodMaxAdcs();

#endif // LIBTMOD_LIBRARY_H
