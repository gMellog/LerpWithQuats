cmake_minimum_required(VERSION 3.20.0)
project(lerpWithQuats CXX)

add_subdirectory(sources)
add_executable(lerpWithQuats main.cpp)

target_link_libraries(lerpWithQuats lerpWithQuatsLib)
