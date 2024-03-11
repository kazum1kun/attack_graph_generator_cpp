cmake_minimum_required(VERSION 3.28)
project(AttackGraphGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(AttackGraphGenerator main.cpp
        src/graph/Node.h
        src/graph/AttackGraph.h
        Generation.h)

target_include_directories(AttackGraphGenerator
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src/graph
)