cmake_minimum_required(VERSION 3.24)
project(pa7 C)

set(CMAKE_C_STANDARD 11)

add_executable(pa7 main.c common.h ipc.h log.c log.h banking.h pa2345.h pa1.h priorityQueue.c priorityQueue.h)

find_library(libruntime libruntime.so paths ${PROJECT_SOURCE_DIR})
target_link_libraries(pa7 ${libruntime})