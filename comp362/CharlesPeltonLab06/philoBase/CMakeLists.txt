cmake_minimum_required(VERSION 3.8)

project(philosophers)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-m64 -g -O0 -D_DEBUG -Wall")

set(SOURCE_FILES philoBase.c)

add_executable(philosophers ${SOURCE_FILES})

find_package(Threads)
target_link_libraries(philosophers ${CMAKE_THREAD_LIBS_INIT})
