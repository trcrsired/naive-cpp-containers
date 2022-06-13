cmake_minimum_required(VERSION 3.22)

project(naivecppcontainers)

set(CMAKE_CXX_STANDARD 23)

INSTALL (DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
