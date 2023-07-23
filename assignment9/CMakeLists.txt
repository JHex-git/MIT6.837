cmake_minimum_required(VERSION 3.10)
project(particle_system)

SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_STANDARD 17)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
message(WARNING ${CMAKE_SOURCE_DIR})
add_library(mylib OBJECT src/forcefield.cpp src/generator.cpp src/glCanvas.cpp src/integrator.cpp src/matrix.cpp src/parser.cpp src/particle.cpp src/system.cpp)
add_executable(particle_system src/main.cpp)
target_link_libraries(particle_system mylib ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

MACRO (COPY_GNU_DLL trgt libname)
ADD_CUSTOM_COMMAND (
    TARGET ${trgt}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different "${GLUT_INCLUDE_DIR}/../bin/${libname}.dll" ${RUNTIME_OUTPUT_DIRECTORY}
)
ENDMACRO (COPY_GNU_DLL)

COPY_GNU_DLL(particle_system libfreeglut)