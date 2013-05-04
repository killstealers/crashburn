CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# MD2 loader test. Usage: ./test_md2 <FILENAME>
ADD_EXECUTABLE(test_md2 $<TARGET_OBJECTS:crashburn_objects>
                        test_md2.cpp)
TARGET_LINK_LIBRARIES(test_md2 ${GLFW_LIBRARY}
                               ${OPENGL_LIBRARIES})

# Test to display test on the screen
ADD_EXECUTABLE(test_drawtext $<TARGET_OBJECTS:crashburn_objects>
                             test_drawtext.cpp)
TARGET_LINK_LIBRARIES(test_drawtext ${GLFW_LIBRARY}
                                    ${OPENGL_LIBRARIES})

# Test to load a tga texture
ADD_EXECUTABLE(test_tga $<TARGET_OBJECTS:crashburn_objects>
                        test_tga.cpp)
TARGET_LINK_LIBRARIES(test_tga ${GLFW_LIBRARY}
                               ${OPENGL_LIBRARIES})

# ===== Template for adding a new test =====
# Create an executable with crashburn object files adding
# a custom entry point
#ADD_EXECUTABLE(test_NAME $<TARGET_OBJECTS:crashburn_objects>
#                         test_NAME.cpp)
#TARGET_LINK_LIBRARIES(test_NAME ${GLFW_LIBRARY}
#                                ${OPENGL_LIBRARIES})
