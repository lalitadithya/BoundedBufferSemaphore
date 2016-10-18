cmake_minimum_required(VERSION 3.6)
project(BoundedBufferSemaphore)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(BoundedBufferSemaphore ${SOURCE_FILES})
target_link_libraries(BoundedBufferSemaphore pthread)
target_link_libraries(BoundedBufferSemaphore rt)