cmake_minimum_required(VERSION 3.10)
project(raytracer)

SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_STANDARD 17)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
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
add_subdirectory(src/opengl)
add_subdirectory(src/lights)
add_subdirectory(src/materials)
add_subdirectory(src/raytrace)
add_executable(raytracer src/main.cpp)
target_link_libraries(raytracer raytrace_lib cameras_lib object3ds_lib utility_lib mygl_lib lights_lib materials_lib ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

MACRO (COPY_GNU_DLL trgt libname)
ADD_CUSTOM_COMMAND (
    TARGET ${trgt}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different "${GLUT_INCLUDE_DIR}/../bin/${libname}.dll" ${RUNTIME_OUTPUT_DIRECTORY}
)
ENDMACRO (COPY_GNU_DLL)

COPY_GNU_DLL(raytracer libfreeglut)