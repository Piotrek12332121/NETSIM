cmake_minimum_required(VERSION 3.19)

project(NETSIM)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Werror=switch)

add_executable(lab7
        main.cpp
        src/storage_types.cpp
        src/package.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        include/types.hpp
        include/package.hpp
        include/storage_types.hpp
        include/nodes.hpp
        include/helpers.hpp
        include/factory.hpp
        include/reports.hpp
        src/reports.cpp
        include/simulation.hpp
        src/simulation.cpp)

target_include_directories(lab7 PUBLIC
        src
        include
        )