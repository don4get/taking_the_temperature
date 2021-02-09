// C includes
#include <fcntl.h>

// C++ Sytem includes
#include <string>
#include <thread>

// Third parties C++ includes
#include <boost/format.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/regex.hpp>

// Own libraries includes
#include "VmeSystem.h"

using namespace std::chrono;

int main()
{
    // Open file output stream properly.
    // See https://security.web.cern.ch/recommendations/en/codetools/cpp.shtml
    std::string filename = "report.yaml";
    int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0600);
    if (fd == -1)
    {
        const std::string errorMessage =
            str(boost::format("Impossible to access %1%.") % filename);
        throw std::invalid_argument(errorMessage);
    }
    io::stream_buffer<io::file_descriptor_sink> fp(
        fd, boost::iostreams::close_handle);
    std::ostream out(&fp);

    // Instantiate Vme system.
    VmeSystem v;

    // Add sensors.
    v.addSensor(1, SensorType::VOLTAGE_0V_10V);
    v.setScalingData(1, 0.5f, 5.f);
    v.addSensor(2, SensorType::CURRENT_4MA_20MA, 3.f, -2.f, "PT1000");

    v.removeSensor(1);
    v.addSensor(3, SensorType::VOLTAGE_0V_10V);

    // Try to remove a non valid sensor address.
    try
    {
        v.removeSensor(4);
    }
    catch (std::invalid_argument& e)
    {
        cout << "The following error is raised for demonstration:";
        cout << e.what();
    }

    // Set output stream
    v.setOutputStream(&out);

    // Get a map, instead of a list, of the temperature sensors registered in
    // the VME system.
    const map<uint16_t, TemperatureSensor>& sensors = v.getTemperatureSensors();
    (void) sensors; // Unused variable

    // Produce report, the supervision is in charge of triggering it.
    // For brevity, report is performed every 100ms instead of 60 seconds.
    const chrono::milliseconds timeWindow = std::chrono::milliseconds(100);
    int i = 0;
    while (i < 10)
    {
        i++;
        time_point<steady_clock> start = std::chrono::steady_clock::now();
        v.measureTemperaturesAndProduceReport();
        time_point<steady_clock> end = std::chrono::steady_clock::now();
        auto elapsed = end - start;

        auto timeToWait = timeWindow - elapsed;
        if (timeToWait > std::chrono::milliseconds::zero())
        {
            std::this_thread::sleep_for(timeToWait);
        }
    }

    // At the end of the application, the VME system object is destroyed
    // and the ostream is closed.

    return 0;
}
