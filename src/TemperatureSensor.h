#ifndef TAKING_THE_TEMPERATURE_TEMPERATURESENSOR_H
#define TAKING_THE_TEMPERATURE_TEMPERATURESENSOR_H

#include <iostream>
#include <string>

#include "tmod.h"

using namespace std;

constexpr float TSEN_DEFAULT_SCALING_FACTOR = 1.f;
constexpr float TSEN_DEFAULT_OFFSET = 0.f;
constexpr char TSEN_DEFAULT_NAME[] = "Unnamed";

enum SensorType
{
    VOLTAGE_0V_10V = 0,  /**< Voltage 0-10V */
    CURRENT_4MA_20MA = 1 /**< Current 4-20 mA */
};

class TemperatureSensor
{
public:
    TemperatureSensor(uint16_t hardwareId, SensorType type,
                      float scalingFactor = TSEN_DEFAULT_SCALING_FACTOR,
                      float offset = TSEN_DEFAULT_OFFSET,
                      string name = TSEN_DEFAULT_NAME);

    /**
     * @brief Measure temperature.
     * Read Adc and convert measurement into temperature in degree Celsius.
     * @return Temperature.
     */
    float measureTemperature();

    /**
     * @brief Get the last temperature measurement, in degree Celsius.
     * @return Temperature [C]
     * @throw std::runtime_error: If not previous Adc measurement has been
     * made.
     * @see measureTemperature()
     */
    [[nodiscard]] float getTemperature() const;

    /**
     * @brief Get the minimum temperature measurement since the sensor is
     * instantiated, in degree Celsius.
     * @return Minimum temperature [C]
     * @throw std::runtime_error: If not previous Adc measurement has been
     * made.
     * @see measureTemperature()
     */
    [[nodiscard]] float getMinTemperature() const;

    /**
     * @brief Get the maximum temperature measurement since the sensor is
     * instantiated, in degree Celsius.
     * @return Maximum temperature [C]
     * @throw std::runtime_error: If not previous Adc measurement has been
     * made.
     * @see measureTemperature()
     */
    [[nodiscard]] float getMaxTemperature() const;

    /**
     * @brief Get the sensor type.
     * @return Sensor type
     * @see SensorType
     */
    [[nodiscard]] SensorType getSensorType() const;

    /**
     * @brief Get the scaling factor.
     * @return Scaling factor
     */
    [[nodiscard]] float getScalingFactor() const;

    /**
     * @brief Get the offset.
     * @return Offset
     */
    [[nodiscard]] float getOffset() const;

    /**
     * @brief Get the last Adc measurement value.
     * @return Adc value
     * @throw std::runtime_error: if hardware Id is not valid.
     */
    [[nodiscard]] int16_t getAdcValue() const;

    /**
     * @brief Get the hardwareId.
     * @return Hardware Id
     */
    [[nodiscard]] uint16_t getHardwareId() const;

    /**
     * @brief Get the sensor name.
     * @return Sensor name
     */
    [[nodiscard]] const string& getName() const;

    /**
     * @brief Set the scaling factor.
     */
    void setScalingFactor(float scalingFactor);

    /**
     * @brief Set the offset.
     */
    void setOffset(float offset);

    /**
     * @brief Compare two sensors.
     * It compares two sensors regarding its name, hardware address and
     * scaling data.
     */
    friend bool operator==(const TemperatureSensor& s1,
                           const TemperatureSensor& s2);

    /**
 * @brief Represent sensor in output stream.
 */
    friend ostream& operator<<(ostream& os, const TemperatureSensor& s);


private:
    /**
     * @brief Hardware address, aka Hardware Id.
     */
    uint16_t hardwareId = UINT16_MAX;
    /**
     * @brief Sensor name.
     * Default name is "Unnamed".
     */
    string name;
    /**
     * @brief Sensor type.
     * @see SensorType
     */
    SensorType sensorType;
    //! Scaling factor.
    float scalingFactor;
    //! Offset.
    float offset;
    /**
     * Adc measurement value.
     * VME crate using unipolar 14-bit ADCs with an input range of 0-10V.
     * The 4-20mA current signals pass through a 500 ohms shunt to produce
     * a voltage (2-10V) that is measured by the ADC.
     * Consistent Adc value is between 0-16383.
     */
    int16_t adcValue = TMOD_INVALID_VOLTAGE_MEASUREMENT;
    int16_t minAdcValue = INT16_MAX;
    int16_t maxAdcValue = TMOD_INVALID_VOLTAGE_MEASUREMENT;
    /**
     * @brief Temperature measurement [C]
     * Initialised to 0 as it cannot be read by the user until an ADC
     * measurement has been done.
     * @see getTemperature()
     */
    float temperature = 0.f;
    /**
     * @brief Minimum temperature measurement [C]
     * Initialised to 0 as it cannot be read by the user until an ADC
     * measurement has been done.
     * @see getMinTemperature()
     */
    float minTemperature = 1e9f;
    /**
     * @brief Temperature measurement [C]
     * Initialised to 0 as it cannot be read by the user until an ADC
     * measurement has been done.
     * @see getMaxTemperature()
     */
    float maxTemperature = -1e9f;

    void readAdcValue();

    void convertAdcValue();
};

#endif // TAKING_THE_TEMPERATURE_TEMPERATURESENSOR_H
