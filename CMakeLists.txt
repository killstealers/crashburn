CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(crashburn)

# ============= FIND EXTERNAL LIBRARIES ==========
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

FIND_PACKAGE(GLFW   REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

# =============== INCLUDES =======================
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# =============== MAIN OBJECTS ===================
# Match everything except the entry point
FILE(GLOB_RECURSE CRASHBURN_SOURCES src/*/*.cpp)

# Just build the object files, so that we could reuse them
# apart from the main executable (e.g. in test)
ADD_LIBRARY(crashburn_objects OBJECT ${CRASHBURN_SOURCES})

# Create an executable by just adding the main entry point
ADD_EXECUTABLE(crashburn $<TARGET_OBJECTS:crashburn_objects>
                         src/main.cpp)

# =============== LINK LIBRARIES =================
TARGET_LINK_LIBRARIES(crashburn ${GLFW_LIBRARY}
                                ${OPENGL_LIBRARIES})

# =============== TESTS ==========================
ADD_SUBDIRECTORY(tests)
