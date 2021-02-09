#include <iostream>

#include <boost/generator_iterator.hpp>
#include <boost/random.hpp>

#include "tmod.h"

#include <chrono>
#include <cstdint>
#include <iostream>

uint64_t timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
        .count();
}

int16_t tmodReadAdc(uint16_t hardwareAddress)
{
    // Hardware address can't be negative as it is uint16_t.
    if (hardwareAddress > tmodMaxAdcs())
        return TMOD_INVALID_VOLTAGE_MEASUREMENT;

    boost::mt19937 rng(timeSinceEpochMillisec());
    boost::uniform_int<> adcMeasurementRange(0, TMOD_MAX_ADC_VALUE);
    boost::variate_generator<boost::mt19937, boost::uniform_int<>>
        randomMeasurement(rng, adcMeasurementRange);
    return randomMeasurement();
}

uint16_t tmodMaxAdcs() { return TMOD_MAX_ADCS; }
