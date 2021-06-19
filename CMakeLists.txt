cmake_minimum_required(VERSION 3.2)

project(argx)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

unset(CMAKE_C_FLAGS_RELEASE)
unset(CMAKE_C_FLAGS_DEBUG)

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_DEBUG "-g")

set(WARNINGS "-Wall -Wextra")

set(CMAKE_C_FLAGS "${WARNINGS}")

set(SRC_DIR "src")
set(INCLUDE_DIR "include")

set(SRC
    ${SRC_DIR}/argx.c
    ${SRC_DIR}/argx.cpp
)

set(HEADERS
    ${INCLUDE_DIR}/argx/argx.h
    ${INCLUDE_DIR}/argx/argx.hpp
)

add_library(${PROJECT_NAME}
    ${SRC}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
)
