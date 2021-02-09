#ifndef TAKING_THE_TEMPERATURE_VMESYSTEM_H
#define TAKING_THE_TEMPERATURE_VMESYSTEM_H

// STD includes
#include <iostream>
#include <map>

// Third parties includes
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream_buffer.hpp>

// Local includes
#include "TemperatureSensor.h"

namespace io = boost::iostreams;
using namespace std;

class VmeSystem
{
public:
    //! Default constructor.
    VmeSystem();

    /**
     * @brief Add a sensor to the VmeSystem.
     *
     * @param hardwareId: the address of the ADC reading the sensor.
     * @param sensorType:  Voltage or current
     * @param scalingFactor: scaling factor for the conversion to a
     * temperature, optional.
     * @param offset: offset for the conversion to a temperature, optional.
     * @param name: name of the sensor, optional.
     */
    //!
    void addSensor(uint16_t hardwareId, SensorType sensorType,
                   float scalingFactor = TSEN_DEFAULT_SCALING_FACTOR,
                   float offset = TSEN_DEFAULT_OFFSET,
                   string name = TSEN_DEFAULT_NAME);

    /**
     * @brief Remove a sensor from the Vme system
     * @param hardwareId: the address of the ADC reading the sensor.
     */
    void removeSensor(uint16_t hardwareId);

    /**
     * @brief Set the scaling data for a given sensor.
     * @param hardwareId: hardware address of the sensor to calibrate.
     * @param scalingFactor: scaling factor.
     * @param offset: offset.
     * @throw invalid_argument: if no sensor is registered at this address.
     */
    void setScalingData(uint16_t hardwareId, float scalingFactor, float offset);

    /**
     * @brief Set the output stream for report generation.
     * @param out: point to the output stream.
     * @throw invalid_argument: if no sensor is registered at this address.
     */
    void setOutputStream(ostream* out);

    /**
     * @brief Measure the temperatures and produce report.
     */
    void measureTemperaturesAndProduceReport();

    /**
     * @brief Get a map of the registred sensors.
     * @return map of the registred sensors.
     * @throw invalid_argument: if output stream is invalid pointer.
     *
     */
    [[nodiscard]] const map<uint16_t, TemperatureSensor>&
    getTemperatureSensors() const;

private:
    /// Sensor temperatures.
    map<uint16_t, TemperatureSensor> temperatureSensors;
    /// Output stream.
    ostream* outputStream;
};

#endif // TAKING_THE_TEMPERATURE_VMESYSTEM_H
