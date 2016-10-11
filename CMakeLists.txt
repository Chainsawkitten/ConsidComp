cmake_minimum_required(VERSION 2.8.12)

project(ConsidComp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -O3 -gdwarf-2 -static-libstdc++ -Wall")
endif()

add_executable(ConsidComp main.cpp)
set_property(TARGET ConsidComp PROPERTY CXX_STANDARD 11)
set_property(TARGET ConsidComp PROPERTY CXX_STANDARD_REQUIRED ON)
