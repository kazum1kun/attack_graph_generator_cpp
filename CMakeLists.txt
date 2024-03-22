cmake_minimum_required(VERSION 3.22.1)
project(AttackGraphGenerator)

set(CMAKE_CXX_STANDARD 20)

add_executable(AttackGraphGenerator src/main.cpp
        src/graph/GraphNode.hpp
        src/graph/AttackGraph.hpp
        src/Generation.hpp
        src/io/GraphToCsv.hpp
        src/io/FsUtils.hpp
        src/io/CsvToPdf.hpp
        src/sat/ShortestTrace.hpp
        src/priority_queue/PriorityQueue.hpp
        src/priority_queue/PriorityQueue.hpp)

find_package(effolkronium_random REQUIRED)
find_package(argparse REQUIRED)


target_include_directories(AttackGraphGenerator
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src/graph
        ${PROJECT_SOURCE_DIR}/src/io
)

target_link_libraries(AttackGraphGenerator
        effolkronium_random
        argparse::argparse
)
