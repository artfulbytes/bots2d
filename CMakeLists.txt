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
    src/engine/renderer/Camera.cpp
    src/engine/renderer/GLError.cpp
    src/engine/renderer/VertexBuffer.cpp
    src/engine/renderer/IndexBuffer.cpp
    src/engine/renderer/VertexArray.cpp
    src/engine/renderer/Shader.cpp
    src/engine/renderer/ImGuiOverlay.cpp
    src/engine/renderer/stb_image.cpp
    src/engine/renderer/Texture.cpp
    src/engine/renderer/Texture.cpp
)

set (PHYSICS_SOURCE_FILES
    src/engine/physics/PhysicsWorld.cpp
    src/engine/physics/Body2D.cpp
    src/engine/physics/RangeSensor.cpp
    src/engine/physics/LineDetector.cpp
    src/engine/physics/ContactListener.cpp
)

set (CONTROLLER_SOURCE_FILES
    src/engine/controller/Microcontroller.cpp
    src/engine/controller/MicrocontrollerCBinding.cpp
)

set (SIMULATOR_ENGINE_FILES
    src/engine/core/Application.cpp
    src/engine/core/Event.cpp
    src/engine/scene/SceneObject.cpp
    src/engine/scene/Scene.cpp
    src/engine/scene/QuadComponent.cpp
    src/engine/scene/SpriteAnimation.cpp
    ${RENDERER_SOURCE_FILES}
    ${PHYSICS_SOURCE_FILES}
    ${CONTROLLER_SOURCE_FILES}
    ${GLAD_GL_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
)

set (SIMULATOR_APP_FILES
    src/app/SimulatorApp.cpp
    src/app/SceneMenu.cpp
    src/app/scenes/AppScene.cpp
    src/app/scenes/DrawTestScene.cpp
    src/app/scenes/SpriteAnimationTestScene.cpp
    src/app/scenes/PhysicsBoxTestScene.cpp
    src/app/scenes/WheelMotorTestScene.cpp
    src/app/scenes/Sumobot4WheelTestScene.cpp
    src/app/objects/TopViewWheelMotor.cpp
    src/app/objects/TopViewSumobot4Wheel.cpp
    src/app/objects/TopViewDohyo.cpp
    src/app/objects/RangeSensorObject.cpp
    src/app/objects/LineDetectorObject.cpp
    src/app/objects/controllers/sumobot4wheelexample/main.c
    src/app/objects/controllers/sumobot4wheelexample/MicrocontrollerSumobot4WheelExample.cpp
)

set (SIMULATOR_SOURCE_FILES
    src/main.cpp
    ${SIMULATOR_ENGINE_FILES}
    ${SIMULATOR_APP_FILES}
)

add_executable(sumobot_simulator
    ${SIMULATOR_SOURCE_FILES}
)

target_include_directories(sumobot_simulator PRIVATE include)
target_include_directories(sumobot_simulator PRIVATE include/app)
target_include_directories(sumobot_simulator PRIVATE include/app/scenes)
target_include_directories(sumobot_simulator PRIVATE include/app/objects)
target_include_directories(sumobot_simulator PRIVATE include/engine/core)
target_include_directories(sumobot_simulator PRIVATE include/engine/renderer)
target_include_directories(sumobot_simulator PRIVATE include/engine/physics)
target_include_directories(sumobot_simulator PRIVATE include/engine/controller)
target_include_directories(sumobot_simulator PRIVATE include/engine/scene)
target_include_directories(sumobot_simulator PRIVATE src/app/objects/controllers)
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
