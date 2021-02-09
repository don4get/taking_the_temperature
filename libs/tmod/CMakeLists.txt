add_library(tmod tmod.cpp tmod.h)
set_target_properties(tmod PROPERTIES PUBLIC_HEADER tmod.h)
target_sources(tmod PUBLIC tmod.h)
target_include_directories(tmod INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})