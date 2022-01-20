cmake_minimum_required(VERSION 3.16)
project(thjson)

add_library(common STATIC common.cpp thdecode.cpp thencode.cpp)

add_executable(threp threp.cpp)
target_link_libraries(threp common)
target_link_libraries(threp -Wl,-subsystem,console)

add_executable(thjson thjson.cpp)
target_link_libraries(thjson common)
target_link_libraries(thjson -Wl,-subsystem,console)