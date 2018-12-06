cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 11)

add_library(transitionTour transitionTour.cpp transitionTour.h)

add_executable(main main.cpp)

target_link_libraries(main transitionTour)

