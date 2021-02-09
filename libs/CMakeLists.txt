add_subdirectory(tmod)
if (NOT ENABLE_COVERAGE)
    add_subdirectory(supervision)
endif ()

set(YAML_CPP_BUILD_CONTRIB CACHE OFF BOOL FORCE)
set(YAML_CPP_BUILD_TOOLS CACHE OFF BOOL FORCE)
set(YAML_CPP_INSTALL CACHE OFF BOOL FORCE)
add_subdirectory(yaml-cpp)