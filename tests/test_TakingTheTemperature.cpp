#define BOOST_TEST_MODULE test_TakingTheTemperature

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/format.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <yaml-cpp/yaml.h>

#include "TemperatureSensor.h"
#include "VmeSystem.h"

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;
using boost::test_tools::output_test_stream;
using std::stringstream;

BOOST_AUTO_TEST_CASE( // NOLINT(cert-err58-cpp)
    test_TemperatureSensor_ValidInputNoAdcReading, *utf::tolerance(0.00001))
{

    int16_t hardwareId = 1;
    SensorType sensorType = SensorType::VOLTAGE_0V_10V;
    TemperatureSensor sensor = TemperatureSensor(hardwareId, sensorType);
    float scalingFactor = TSEN_DEFAULT_SCALING_FACTOR;
    float offset = TSEN_DEFAULT_OFFSET;

    BOOST_TEST(sensor.getHardwareId() == hardwareId);
    BOOST_TEST(sensor.getSensorType() == sensorType);
    BOOST_TEST(sensor.getScalingFactor() == scalingFactor, tt::tolerance(0.0));
    BOOST_TEST(sensor.getOffset() == TSEN_DEFAULT_OFFSET, tt::tolerance(0.0));
    BOOST_TEST(sensor.getName() == TSEN_DEFAULT_NAME);
    BOOST_CHECK_THROW([[maybe_unused]]float a = sensor.getTemperature(), runtime_error);
    BOOST_CHECK_THROW([[maybe_unused]]float b = sensor.getMinTemperature(), runtime_error);
    BOOST_CHECK_THROW([[maybe_unused]]float c = sensor.getMaxTemperature(), runtime_error);
    BOOST_CHECK_THROW([[maybe_unused]]float d = sensor.getAdcValue(), runtime_error);

    scalingFactor = 2.f;
    sensor.setScalingFactor(scalingFactor);
    BOOST_TEST(sensor.getScalingFactor() == scalingFactor, tt::tolerance(0.0));

    offset = -5.f;
    sensor.setOffset(offset);
    BOOST_TEST(sensor.getOffset() == offset, tt::tolerance(0.0));
}

BOOST_AUTO_TEST_CASE( // NOLINT(cert-err58-cpp)
    test_TemperatureSensor_TooHighHardwareId, *utf::tolerance(0.00001))
{
    int16_t hardwareId = TMOD_MAX_ADCS + 1;
    SensorType sensorType = SensorType::VOLTAGE_0V_10V;

    BOOST_CHECK_THROW(TemperatureSensor(hardwareId, sensorType),
                      invalid_argument);
}

BOOST_AUTO_TEST_CASE( // NOLINT(cert-err58-cpp)
    test_TemperatureSensor_ValidInputWithAdcReading, *utf::tolerance(0.00001))
{

    int16_t hardwareId = 2;
    SensorType sensorType = SensorType::VOLTAGE_0V_10V;
    float scalingFactor = 0.4f;
    float offset = 2.f;
    string name = "Coolant Temperature";
    TemperatureSensor sensor =
        TemperatureSensor(hardwareId, sensorType, scalingFactor, offset, name);

    BOOST_TEST(sensor.getScalingFactor() == 0.4f, tt::tolerance(0.0));
    BOOST_TEST(sensor.getOffset() == 2.f, tt::tolerance(0.0));
    BOOST_TEST(sensor.getName() == name);
    BOOST_TEST(sensor.getHardwareId() == 2);

    float temperature = sensor.measureTemperature();

    float expectedTemperature =
        (float)sensor.getAdcValue() * scalingFactor + offset;

    BOOST_TEST(temperature == expectedTemperature);
    BOOST_TEST(sensor.getTemperature() == expectedTemperature);

    scalingFactor = 0.f;
    offset = -5.f;
    sensor.setScalingFactor(scalingFactor);
    sensor.setOffset(offset);

    expectedTemperature = TMOD_DEFAULT_ADC_VALUE * scalingFactor + offset;
    BOOST_TEST(sensor.getTemperature() == expectedTemperature);
    BOOST_TEST(sensor.getMinTemperature() == expectedTemperature);
    BOOST_TEST(sensor.getMaxTemperature() == expectedTemperature);

    float expectedTemperature2 =
        (float)sensor.getAdcValue() * scalingFactor + offset;

    float minTemperature = min(expectedTemperature, expectedTemperature2);
    float maxTemperature = max(expectedTemperature, expectedTemperature2);

    BOOST_TEST(sensor.getTemperature() == expectedTemperature2);
    BOOST_TEST(sensor.getMinTemperature() == minTemperature);
    BOOST_TEST(sensor.getMaxTemperature() == maxTemperature);
}

BOOST_AUTO_TEST_CASE(test_VmeSystem_ValidInput, // NOLINT(cert-err58-cpp)
                     *utf::tolerance(0.00001))
{

    const uint16_t hardwareId = 1;
    SensorType sensorType = SensorType::VOLTAGE_0V_10V;
    float scalingFactor = 0.f;
    float offset = -1.f;

    VmeSystem vmeSystem = VmeSystem();
    vmeSystem.addSensor(hardwareId, sensorType);
    vmeSystem.setScalingData(1, scalingFactor, offset);

    TemperatureSensor expectedSensor(hardwareId, sensorType, scalingFactor,
                                     offset);

    BOOST_TEST(vmeSystem.getTemperatureSensors().size() == 1);
    BOOST_TEST(vmeSystem.getTemperatureSensors().at(hardwareId) ==
               expectedSensor);

    vmeSystem.removeSensor(hardwareId);
    BOOST_TEST(vmeSystem.getTemperatureSensors().size() == 0);

    BOOST_CHECK_THROW(vmeSystem.removeSensor(4), invalid_argument);

    auto output = new output_test_stream();
    vmeSystem.setOutputStream(output);

    vmeSystem.addSensor(hardwareId, sensorType);
    vmeSystem.addSensor(2, SensorType::CURRENT_4MA_20MA);

    vmeSystem.measureTemperaturesAndProduceReport();
    vmeSystem.measureTemperaturesAndProduceReport();

    string report = output->str();
    YAML::Node reportNode = YAML::Load(report);
    BOOST_TEST(reportNode.IsMap());
    BOOST_TEST(reportNode.size() == 2);
    for (YAML::const_iterator i = reportNode.begin(); i != reportNode.end();
         ++i)
    {
        YAML::Node sensorsNode = i->second;

        BOOST_TEST(sensorsNode.IsMap());
        BOOST_TEST(sensorsNode.size() == 2);

        for (YAML::const_iterator j = sensorsNode.begin();
             j != sensorsNode.end(); ++j)
        {
            const map<uint16_t, TemperatureSensor>& sensors =
                vmeSystem.getTemperatureSensors();

            YAML::Node sensorNode = j->second;
            auto rhardwareId = sensorNode["Hardware Id"].as<uint16_t>();
            auto rname = sensorNode["Name"].as<string>();
            string expectedName = sensors.at(rhardwareId).getName();
            auto sf = sensorNode["Scaling factor"].as<float>();
            float expectedSF = sensors.at(rhardwareId).getScalingFactor();
            auto off = sensorNode["Offset"].as<float>();
            float expectedOffset = sensors.at(rhardwareId).getOffset();

            BOOST_TEST(rname == expectedName);
            BOOST_TEST(sf == expectedSF);
            BOOST_TEST(off == expectedOffset);
        }
    }
}

BOOST_AUTO_TEST_CASE( // NOLINT(cert-err58-cpp)
    test_VmeSystem_TooHighHardwareId, *utf::tolerance(0.00001))
{
    int16_t hardwareId = TMOD_MAX_ADCS + 1;
    SensorType sensorType = SensorType::VOLTAGE_0V_10V;

    VmeSystem vmeSystem = VmeSystem();

    BOOST_CHECK_THROW(vmeSystem.addSensor(hardwareId, sensorType),
                      std::invalid_argument);
}
