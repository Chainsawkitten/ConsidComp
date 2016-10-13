cmake_minimum_required(VERSION 2.8.12)

project(ConsidComp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -O3 -gdwarf-2 -static-libstdc++ -Wall")
endif()

add_executable(ConsidCompTest main.cpp)
set_property(TARGET ConsidCompTest PROPERTY CXX_STANDARD 11)
set_property(TARGET ConsidCompTest PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(ConsidComp ConsidComp.cpp)
set_property(TARGET ConsidComp PROPERTY CXX_STANDARD 11)
set_property(TARGET ConsidComp PROPERTY CXX_STANDARD_REQUIRED ON)
