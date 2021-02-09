add_library(ttt)
target_sources(ttt
        PUBLIC
        TemperatureSensor.h
        VmeSystem.h
        PRIVATE
        TemperatureSensor.cpp
        VmeSystem.cpp
        )
target_link_libraries(ttt
        PUBLIC
        ${Boost_LIBRARIES}
        tmod
        yaml-cpp)
target_include_directories(tmod INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
