cmake_minimum_required(VERSION 3.8)
project(liblev)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/lev.h src/loop.c src/event.c src/queue.h src/epoll.c src/queue.c src/io.h src/utils.c src/utils.h)
add_library(libed SHARED ${SOURCE_FILES} src/loop.c src/event.c src/epoll.c src/queue.c src/utils.c)