cmake_minimum_required(VERSION 3.10)
project(curve_editor)

SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_STANDARD 17)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
message(WARNING ${CMAKE_SOURCE_DIR})
add_library(mylib OBJECT curve.cpp glCanvas.cpp matrix.cpp spline_parser.cpp triangle_mesh.cpp surface.cpp)
add_executable(curve_editor main.cpp)
target_link_libraries(curve_editor mylib ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

MACRO (COPY_GNU_DLL trgt libname)
ADD_CUSTOM_COMMAND (
    TARGET ${trgt}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different "${GLUT_INCLUDE_DIR}/../bin/${libname}.dll" ${RUNTIME_OUTPUT_DIRECTORY}
)
ENDMACRO (COPY_GNU_DLL)

COPY_GNU_DLL(curve_editor libfreeglut)