cmake_minimum_required(VERSION 3.5)
project(okssh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")


set(SOURCE_FILES main.cpp okssh.h okssh.cpp)

#import cpp-yaml
find_package(PkgConfig)
find_package(Boost)
pkg_check_modules(PC_YAML_CPP REQUIRED yaml-cpp)
#pkg_search_module(PC_YAML_CPP REQUIRED yaml-cpp)



include_directories(${PC_YAML_CPP_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_directories(${PC_YAML_CPP_LIBRARY_DIRS})


add_executable(okssh ${SOURCE_FILES})
target_link_libraries(okssh ${PC_YAML_CPP_LIBRARIES})


install(TARGETS okssh DESTINATION bin)
