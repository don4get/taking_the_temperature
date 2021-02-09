add_executable(supervision supervision.cpp)
target_link_libraries(supervision
        PUBLIC
        ${Boost_LIBRARIES}
        yaml-cpp
        ttt)