cmake_minimum_required(VERSION 3.10)
project(untitled2 C)

set(CMAKE_C_STANDARD 99)

add_executable(
        exec2
        main2.c
)

include_directories(obj)

add_executable(untitled2
        main.c
        nisteFunctii.c
        unHeaderDubios.h
        obj/thread_module/my_thread.h
        obj/thread_module/my_thread.c
        obj/thread_module/my_thread_defs.h)

target_link_libraries(
        untitled2
        -lpthread
)
