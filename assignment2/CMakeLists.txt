cmake_minimum_required(VERSION 3.10)
project(raytracer)

SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_STANDARD 17)

# file(GLOB SRCS src/**.cpp)
# SET(LIB_SRCS ${SRCS})
# list(FILTER LIB_SRCS EXCLUDE REGEX "main.cpp$")

# include_directories(src/)
# add_executable(raytracer src/main.cpp)
# add_library(linear_algebra_lib OBJECT ${LIB_SRCS})
# target_link_libraries(ifs ifs_lib)
include_directories(src/)
add_subdirectory(src/cameras)
add_subdirectory(src/object3ds)
add_subdirectory(src/utility)
add_executable(raytracer src/main.cpp)
target_link_libraries(raytracer cameras_lib object3ds_lib utility_lib)