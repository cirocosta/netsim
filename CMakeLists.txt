# setup
cmake_minimum_required(VERSION 2.8)
option(netsim_tests "Include Tests." ON)

set(PROJECT_NAME netsim)
project(${PROJECT_NAME} C)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

# deps
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
set(LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

# src

add_subdirectory(src/)

# global vars
SET(${PROJECT_NAME}_INCLUDE_DIRS ${INCLUDE_DIRS}
  CACHE INTERNAL
  "${PROJECT_NAME} Include Directories"
  FORCE
)

SET(${PROJECT_NAME}_LIBRARIES ${LIBRARIES}
  CACHE INTERNAL
  "${PROJECT_NAME} Link Libraries"
  FORCE
)

# test
if (${netsim_tests})
  enable_testing()
  add_subdirectory(tests/)
endif()


