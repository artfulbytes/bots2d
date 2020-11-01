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

set (RENDERER_SOURCE_FILES
    src/engine/renderer/Renderer.cpp
    src/engine/renderer/GLError.cpp
    src/engine/renderer/VertexBuffer.cpp
    src/engine/renderer/IndexBuffer.cpp
    src/engine/renderer/VertexArray.cpp
    src/engine/renderer/Shader.cpp
    src/engine/renderer/ImGuiOverlay.cpp
    src/engine/renderer/stb_image.cpp
    src/engine/renderer/Texture.cpp
)

set (PHYSICS_SOURCE_FILES
    src/engine/physics/SumobotTest.cpp
    src/engine/physics/Sumobot4Wheel.cpp
    src/engine/physics/WheelMotor.cpp
    src/engine/physics/Dohyo.cpp
    src/engine/physics/ContactListener.cpp
    src/engine/physics/RangeSensor.cpp
    src/engine/physics/LineDetector.cpp
    src/engine/physics/BoxObstacle.cpp
)

set (SIMULATOR_ENGINE_FILES
    src/engine/core/Application.cpp
    ${RENDERER_SOURCE_FILES}
    ${PHYSICS_SOURCE_FILES}
    ${GLAD_GL_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
)

set (TEST_SOURCE_FILES
    src/tests/Test.cpp
    src/tests/TestClearColor.cpp
    src/tests/TestDrawBasicShapes.cpp
)

set (SIMULATOR_SOURCE_FILES
    ${SIMULATOR_ENGINE_FILES}
    ${TEST_SOURCE_FILES}
    src/SimulatorApp.cpp
    src/main.cpp
)

add_executable(sumobot_simulator
    ${SIMULATOR_SOURCE_FILES}
)

target_include_directories(sumobot_simulator PRIVATE include)
# TODO: Don't use full path for these?
target_include_directories(sumobot_simulator PRIVATE include/engine/core)
target_include_directories(sumobot_simulator PRIVATE include/engine/physics)
target_include_directories(sumobot_simulator PRIVATE include/engine/renderer)
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
