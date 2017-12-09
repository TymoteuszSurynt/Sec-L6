cmake_minimum_required(VERSION 3.8)
project(Sec_L6)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES bserver.cpp)
set(SOURCE_FILES bclient.cpp)
set(SOURCE_FILES bvrfy.cpp)
add_executable(Server bserver.cpp)
add_executable(Client bclient.cpp)
add_executable(Verify bvrfy.cpp)