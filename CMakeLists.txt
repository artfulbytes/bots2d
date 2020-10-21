cmake_minimum_required(VERSION 3.8)
project(sumobot_simulator VERSION 1.0)

# Get glad from inside GLFW
set(GLAD_GL_SOURCE_FILES
    external/glfw/deps/glad_gl.c
)

set(IMGUI_SOURCE_FILES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
)

# TODO: Divide this into more sets

set (RENDERER_SOURCE_FILES
    src/renderer/Renderer.cpp
    src/renderer/GLError.cpp
    src/renderer/VertexBuffer.cpp
    src/renderer/IndexBuffer.cpp
    src/renderer/VertexArray.cpp
    src/renderer/Shader.cpp
    src/renderer/ImGuiOverlay.cpp
    src/renderer/stb_image.cpp
    src/renderer/Texture.cpp
)

set (PHYSICS_SOURCE_FILES
    src/physics/SumobotTest.cpp
    src/physics/Sumobot4Wheel.cpp
    src/physics/WheelMotor.cpp
    src/physics/Dohyo.cpp
    src/physics/ContactListener.cpp
    src/physics/RangeSensor.cpp
    src/physics/LineDetector.cpp
    src/physics/BoxObstacle.cpp
)

set (TEST_SOURCE_FILES
    src/tests/Test.cpp
    src/tests/TestClearColor.cpp
    src/tests/TestDrawBasicShapes.cpp
)

set (SIMULATOR_SOURCE_FILES
    src/Application.cpp
    ${RENDERER_SOURCE_FILES}
    ${PHYSICS_SOURCE_FILES}
    ${TEST_SOURCE_FILES}
)

add_executable(sumobot_simulator
    ${SIMULATOR_SOURCE_FILES}
    ${GLAD_GL_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
)

target_include_directories(sumobot_simulator PRIVATE include)
target_include_directories(sumobot_simulator PRIVATE include/physics)
target_include_directories(sumobot_simulator PRIVATE include/renderer)
target_include_directories(sumobot_simulator PRIVATE external/glfw/deps)
target_include_directories(sumobot_simulator PRIVATE external/stb)
target_include_directories(sumobot_simulator PRIVATE external/glm)
target_include_directories(sumobot_simulator PRIVATE external/imgui)

# Build box2d and glfw as static libs
add_subdirectory(external/glfw)
target_link_libraries(sumobot_simulator PRIVATE glfw)
# Path to Box2D src to only build Box2D (e.g. not testbed)
add_subdirectory(external/Box2D/src)
target_link_libraries(sumobot_simulator PRIVATE box2d)

# Make Dear ImGui use GLAD2
add_definitions( -DIMGUI_IMPL_OPENGL_LOADER_GLAD2 )

set_target_properties(sumobot_simulator PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
