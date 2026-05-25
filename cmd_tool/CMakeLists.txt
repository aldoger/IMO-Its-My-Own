cmake_minimum_required(VERSION 3.15)
project(imo-tool CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(imo-tool main.cpp)

install(TARGETS imo-tool DESTINATION bin)
