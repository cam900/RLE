cmake_minimum_required(VERSION 3.20.0)
project(libRLE_TEST VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(libRLE_TEST src/main.cpp)
target_include_directories(libRLE_TEST SYSTEM PRIVATE "src/librle/")
