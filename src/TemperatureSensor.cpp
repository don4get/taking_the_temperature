// STD includes
#include <algorithm>
#include <stdexcept>

// Third parties includes
#include <boost/format.hpp>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/node.h>

// Local includes
#include "TemperatureSensor.h"

using namespace std;

TemperatureSensor::TemperatureSensor(uint16_t hardwareId, SensorType sensorType,
                                     float scalingFactor, float offset,
                                     string name)
    : hardwareId(hardwareId), name(move(name)), sensorType(sensorType),
      scalingFactor(scalingFactor), offset(offset)
{
    // Hardware Id cannot be negative, as an uint16.
    if (this->hardwareId >= TMOD_MAX_ADCS)
    {
        string errorMessage = str(
            boost::format("Hardware Id (%1%) should be between 0 and %2%.") %
            hardwareId % TMOD_MAX_ADCS);
        throw invalid_argument(errorMessage);
    }
}

void TemperatureSensor::convertAdcValue()
{
    if (adcValue == -1)
    {
        const string errorMessage =
            str(boost::format(
                    "No ADC reading has been made yet on sensor ID %1%.") %
                hardwareId);
        throw runtime_error(errorMessage);
    }

    temperature = scalingFactor * (float)adcValue + offset;
    minTemperature = scalingFactor * (float)minAdcValue + offset;
    maxTemperature = scalingFactor * (float)maxAdcValue + offset;
}

void TemperatureSensor::readAdcValue()
{
    if (this->hardwareId >= TMOD_MAX_ADCS)
    {
        const string errorMessage =
            str(boost::format(
                    "Sensor hardware ID (%1%) is should be inferior to %2%.") %
                hardwareId % TMOD_MAX_ADCS);
        throw runtime_error(errorMessage);
    }

    adcValue = tmodReadAdc(hardwareId);
    minAdcValue = min(adcValue, minAdcValue);
    maxAdcValue = max(adcValue, maxAdcValue);


    if (adcValue > TMOD_MAX_ADC_VALUE)
    {
        const string errorMessage =
            str(boost::format("Adc Value for temperature sensor ID (%1%) is "
                              "too high: %2% > %3% ") %
                hardwareId % adcValue % TMOD_MAX_ADC_VALUE);
        throw runtime_error(errorMessage);
    }
}

float TemperatureSensor::measureTemperature()
{
    this->readAdcValue();
    this->convertAdcValue();

    if (adcValue == -1)
    {
        const string errorMessage =
            str(boost::format(
                    "No ADC reading has been made yet on sensor ID %1%.") %
                hardwareId);
        throw runtime_error(errorMessage);
    }
    return temperature;
}

SensorType TemperatureSensor::getSensorType() const { return sensorType; }

float TemperatureSensor::getScalingFactor() const { return scalingFactor; }

float TemperatureSensor::getOffset() const { return offset; }

int16_t TemperatureSensor::getAdcValue() const
{
    if (adcValue == -1)
    {
        const string errorMessage =
            str(boost::format(
                    "No ADC reading has been made yet on sensor ID %1%.") %
                hardwareId);
        throw runtime_error(errorMessage);
    }

    return adcValue;
}

uint16_t TemperatureSensor::getHardwareId() const { return hardwareId; }

const string& TemperatureSensor::getName() const { return name; }

/**
 * @brief Return the converted Adc value representing a temperature, in
 * degree Celsius.
 * @throw runtime_error in case no previous ADC reading has been made.
 * @return temperature [C]
 */
float TemperatureSensor::getTemperature() const
{
    if (adcValue == -1)
    {
        const string errorMessage =
            str(boost::format(
                    "No ADC reading has been made yet on sensor ID %1%.") %
                hardwareId);
        throw runtime_error(errorMessage);
    }

    return temperature;
}

void TemperatureSensor::setScalingFactor(float newscalingFactor)
{
    if (scalingFactor != newscalingFactor)
    {
        scalingFactor = newscalingFactor;
        try
        {
            convertAdcValue();
        }
        catch (const runtime_error& e)
        {
            // Catch error as user may want to set scaling factor before
            // reading the Adc.
        }
    }
}

void TemperatureSensor::setOffset(float newOffset)
{
    if (offset != newOffset)
    {
        offset = newOffset;
        try
        {
            convertAdcValue();
        }
        catch (const runtime_error& e)
        {
            // Catch error as user may want to set scaling factor before
            // reading the Adc.
        }
    }
}

bool operator==(const TemperatureSensor& s1, const TemperatureSensor& s2)
{
    return s1.getName() == s2.getName() &&
           s1.getHardwareId() == s2.getHardwareId() &&
           s1.getScalingFactor() == s2.getScalingFactor() &&
           s1.getOffset() == s2.getOffset();
}

float TemperatureSensor::getMinTemperature() const
{
    if (adcValue == -1)
    {
        const string errorMessage =
            str(boost::format(
                    "No ADC reading has been made yet on sensor ID %1%.") %
                hardwareId);
        throw runtime_error(errorMessage);
    }

    return minTemperature;
}

float TemperatureSensor::getMaxTemperature() const
{
    if (adcValue == -1)
    {
        const string errorMessage =
            str(boost::format(
                    "No ADC reading has been made yet on sensor ID %1%.") %
                hardwareId);
        throw runtime_error(errorMessage);
    }

    return maxTemperature;
}

ostream& operator<<(ostream& os, const TemperatureSensor& s)
{

    YAML::Node node;

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "name";
    out << YAML::Value << s.name;
    out << YAML::Key << "Hardware Id";
    out << YAML::Value << s.hardwareId;
    out << YAML::Key << "Scaling factor";
    out << YAML::Value << s.scalingFactor;
    out << YAML::Key << "Offset";
    out << YAML::Value << s.offset;
    out << YAML::EndMap;

    return os << out.c_str();
}