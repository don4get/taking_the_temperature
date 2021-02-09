// STD includes
#include <string>
#include <utility>

// Third parties includes
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <yaml-cpp/emitter.h>

// Local includes
#include "VmeSystem.h"

using namespace boost::posix_time;
using namespace YAML;
using namespace std;

namespace io = boost::iostreams;

VmeSystem::VmeSystem() { outputStream = &cout; }

void VmeSystem::addSensor(uint16_t hardwareId, SensorType sensorType,
                          float scalingFactor, float offset, string name)
{

    TemperatureSensor sensor(hardwareId, sensorType, scalingFactor, offset,
                             move(name));

    temperatureSensors.insert(
        pair<uint16_t, TemperatureSensor>(sensor.getHardwareId(), sensor));
}

void VmeSystem::removeSensor(uint16_t hardwareId)
{
    if (temperatureSensors.find(hardwareId) == temperatureSensors.end())
    {
        const string errorMessage =
            str(boost::format("No Temperature has previously been added to the "
                              "hardware address %1%.") %
                hardwareId);
        throw invalid_argument(errorMessage);
    }
    else
        temperatureSensors.erase(hardwareId);
}

void VmeSystem::setScalingData(uint16_t hardwareId, float scalingFactor,
                               float offset)
{
    if (temperatureSensors.find(hardwareId) == temperatureSensors.end())
    {
        const string errorMessage =
            str(boost::format("No Temperature has previously been added to the "
                              "hardware address %1%.") %
                hardwareId);
        throw invalid_argument(errorMessage);
    }
    else
    {
        temperatureSensors.at(hardwareId).setScalingFactor(scalingFactor);
        temperatureSensors.at(hardwareId).setOffset(offset);
    }
}

void VmeSystem::setOutputStream(ostream* os) { outputStream = os; }

void VmeSystem::measureTemperaturesAndProduceReport()
{

    if (outputStream == nullptr)
    {
        throw invalid_argument("Output stream is null.");
    }

    Emitter out;
    //    *outputStream << "Temperature sensors:";
    auto currentTime = second_clock::local_time();
    string currentTimeStr = to_simple_string(currentTime);
    out << BeginMap;
    out << Key << currentTimeStr << Value << BeginMap;
    for (auto& [unused, value] : temperatureSensors)
    {
        (void)unused; // unused variable
        out << Key
            << std::to_string(value.getHardwareId()) + "-" + value.getName();
        out << Value;
        out << BeginMap;
        out << Key << "Hardware Id";
        out << Value << value.getHardwareId();
        out << Key << "Name";
        out << Value << value.getName();
        out << Key << "Sensor type";
        string sensorType = value.getSensorType() == SensorType::VOLTAGE_0V_10V
                                ? "Voltage 0-10V"
                                : "Current 4-20mA";
        out << Value << sensorType;
        out << Key << "Scaling factor";
        out << Value << value.getScalingFactor();
        out << Key << "Offset";
        out << Value << value.getOffset();
        out << Key << "Current time";
        out << Value << currentTimeStr;
        out << Key << "Temperature";
        out << Value << value.measureTemperature();
        out << Key << "Min temperature";
        out << Value << value.getMinTemperature();
        out << Key << "Max temperature";
        out << Value << value.getMaxTemperature();
        out << EndMap;
    }
    out << EndMap;
    out << EndMap;
    *outputStream << out.c_str() << "\n";
}

const map<uint16_t, TemperatureSensor>& VmeSystem::getTemperatureSensors() const
{
    return temperatureSensors;
}
