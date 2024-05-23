cmake_minimum_required(VERSION 3.11)
project(Learnings)

set(CMAKE_CXX_STANDARD 14)

add_executable(Learnings
        src/directedGraph.cxx
      #  toposort1.cpp
      #  toposort1.h

        src/quickTour.cpp
        src/quickTour.h)

# Dependency to spdlog
target_include_directories(${PROJECT_NAME} PRIVATE src/)