cmake_minimum_required(VERSION 3.5)
project(okssh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})


set(SOURCE_FILES main.cpp)

add_executable(okssh ${SOURCE_FILES})
target_link_libraries(okssh ${CURSES_LIBRARIES})


