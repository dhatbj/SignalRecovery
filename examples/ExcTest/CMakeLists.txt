cmake_minimum_required(VERSION 3.5)
project(ExcTest)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_C_SOURCE=199309L")

add_executable(ExcTest main.cpp ../../signal_recovery.h ../../signal_recovery.c)

target_link_libraries(
        ExcTest
        pthread
)