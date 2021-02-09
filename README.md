# Taking the temperature

[![Travis](https://travis-ci.com/don4get/taking_the_temperature.svg?branch=master)](https://travis-ci.com/don4get/taking_the_temperature.svg?branch=master)
[![Codecov](https://codecov.io/gh/don4get/taking_the_temperature/branch/master/graph/badge.svg?token=MQB9Z9L60D)](https://codecov.io/gh/don4get/taking_the_temperature)

![ThermometerIcon](https://raw.githubusercontent.com/don4get/taking_the_temperature/master/docs/figures/thermometer_icon_250.png)

This Github repository contains a minimalist temperature survey 
application, implemented in C++17.

The application is meant to manage a VME system with temperature sensors. 
The interface implements the following functionalities:
* add a sensor to the VME system;
* remove a sensor from the VME system;
* set a scaling factor and offset for a specific sensor;
* get the list (or the map) of sensors registered on the VME system;
* set the output stream for report;
* read the sensors temperatures;
* produce a sensors temperatures report;
* stop the application.

The repository is organized as follows:
* [libs](https://github.com/don4get/taking_the_temperature/blob/master/libs): contains the required dependencies, i.e. yaml-cpp and libtmod;
* [src](https://github.com/don4get/taking_the_temperature/blob/master/src): contains the source code of the project;
* [src/tests](https://github.com/don4get/taking_the_temperature/blob/master/src/tests): contains basic unit tests for testing the implemented functionalities;
* [docs](https://github.com/don4get/taking_the_temperature/blob/master/docs): contains the documentation of the project, generated with 
  doxygen.

## Requirements
The project depends on:
- CMake >= 3.17
- Boost >= 1.63
- GCC >= 7.3.0
- yaml-cpp, as git submodule

## Architecture
The dependency diagram is illustrated in the figure below.

![DependencyDiagram](https://github.com/don4get/taking_the_temperature/blob/master/docs/figures/ttt_dependency_diagram.png?raw=true)

The `TemperatureSensor` class reads and stores temperature measurements.
The `VmeSystem` class contains a map of `temperatureSensors` registered by 
`hardwareId`.

An example of how to use the interface is written in [libs/libsup/supervision.
cpp](https://github.com/don4get/taking_the_temperature/blob/master/libs
/supervision/supervision.cpp).


## How to install
Type the following in the terminal:
```
mkdir build
cd build
cmake .. -DENABLE_TESTING=1
make -j 2
make test
./supervision # To launch the simple supervision app example, generating a report.yaml.
```

## Documentation
For the description of the interface functions, please refer to the [documentation](https://don4get.github.io/taking_the_temperature/).

The interface allows to produce a report with the temperature readings and saves it to a `yaml` file. 
An example is available at [examples/report.yaml](https://github.com/don4get/taking_the_temperature/blob/master/examples/report.yaml).
The temperature data is saved according to the following structure:
* Date and time of the reading;
* List of sensors connected to the VME system at that time. They are identified by their address and name;
* Information related to the sensor. Namely:
  * hardware ID;
  * name;
  * type;
  * scaling factor;
  * offset;
  * time of measurement;
  * temperature;
  * minimum temperature (since the sensor was added);
  * maximum temperature (since the sensor was added).
  
In code this is equivalent to:
```
2021-Feb-07 18:51:37:
  1_PT1000:
    Hardware Id: 1
    Name: PT1000
    Sensor type: Current 4-20mA
    Scaling factor: 3
    Offset: -2
    Current time: 2021-Feb-07 18:51:37
    Temperature: 11836
    Min temperature: 11836
    Max temperature: 11836
    ...
```

## Contact
For complementary information, please email [Anthony De Bortoli](
mailto:anthony.debortoli@protonmail.com?subject=[Github]%20Taking%20the%20temperature).

## License
This software is provided under MIT License (MIT). Copyright (c) 2021 Anthony De Bortoli.
