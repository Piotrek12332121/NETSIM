cmake_minimum_required(VERSION 3.19)

project(NETSIM)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab7 main.cpp include/package.hpp include/storage_types.hpp include/types.hpp src/storage_types.cpp src/package.cpp include/nodes.hpp include/helpers.hpp include/factory.hpp src/nodes.cpp src/helpers.cpp src/factory.cpp)

target_include_directories(lab7 PUBLIC
        src
        include
        )