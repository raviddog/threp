cmake_minimum_required(VERSION 3.16)
project(thjson)

add_library(common STATIC common.cpp th06.cpp th07.cpp th08.cpp th09.cpp th10.cpp th11.cpp th12.cpp th125.cpp th128.cpp th13.cpp)

add_executable(threp threp.cpp)
target_link_libraries(threp common)
target_link_libraries(threp -Wl,-subsystem,console -g)

add_executable(thjson thjson.cpp)
target_link_libraries(thjson common)
target_link_libraries(thjson -Wl,-subsystem,console -g)