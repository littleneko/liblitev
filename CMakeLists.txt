cmake_minimum_required(VERSION 3.5)
project(liblev)

INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILES (sys/epoll.h HAVE_EPOLL_H)
CHECK_INCLUDE_FILES (sys/event.h HAVE_KQUEUE_H)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(CMAKE_C_STANDARD 99)

include_directories("${PROJECT_BINARY_DIR}")
aux_source_directory(src DIR_SRCS)

add_library(libed SHARED ${DIR_SRCS})
