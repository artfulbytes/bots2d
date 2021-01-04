cmake_minimum_required(VERSION 3.8)
project(bots2d VERSION 1.0)

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
    src/engine/renderer/Scalebar.cpp
    src/engine/renderer/GLError.cpp
    src/engine/renderer/ResourcesHelper.cpp
    src/engine/renderer/VertexBuffer.cpp
    src/engine/renderer/IndexBuffer.cpp
    src/engine/renderer/VertexArray.cpp
    src/engine/renderer/Shader.cpp
    src/engine/renderer/ImGuiOverlay.cpp
    src/engine/renderer/Texture.cpp
    src/engine/renderer/components/RectComponent.cpp
    src/engine/renderer/components/QuadComponent.cpp
    src/engine/renderer/SpriteAnimation.cpp
    src/engine/renderer/stb_image.cpp
)

set (PHYSICS_SOURCE_FILES
    src/engine/physics/PhysicsWorld.cpp
    src/engine/physics/ContactListener.cpp
    src/engine/physics/components/Body2D.cpp
    src/engine/physics/components/RangeSensor.cpp
    src/engine/physics/components/LineDetector.cpp
)

set (CONTROLLER_SOURCE_FILES
    src/engine/controllers/Microcontroller.cpp
    src/engine/controllers/MicrocontrollerCBinding.cpp
)

set (SIMULATOR_ENGINE_FILES
    src/engine/core/Application.cpp
    src/engine/core/Event.cpp
    src/engine/scene/SceneObject.cpp
    src/engine/scene/Scene.cpp
    src/engine/scene/SceneMenu.cpp
    ${RENDERER_SOURCE_FILES}
    ${PHYSICS_SOURCE_FILES}
    ${CONTROLLER_SOURCE_FILES}
    ${GLAD_GL_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
)

set (SIMULATOR_APP_FILES
    src/app/objects/shapes/RectObject.cpp
    src/app/objects/shapes/QuadObject.cpp
    src/app/objects/shapes/CircleObject.cpp
    src/app/objects/shapes/LineObject.cpp
    src/app/objects/actuators/WheelMotor.cpp
    src/app/objects/chassis/GenericBody.cpp
    src/app/objects/robots/BaseBot.cpp
    src/app/objects/robots/Sumobot.cpp
    src/app/objects/robots/LineFollower.cpp
    src/app/objects/sensors/RangeSensorObject.cpp
    src/app/objects/sensors/LineDetectorObject.cpp
    src/app/objects/playgrounds/Dohyo.cpp
    src/app/objects/playgrounds/LineFollowerPath.cpp
    src/app/controllers/Sumobot4WheelExample/main.c
    src/app/controllers/Sumobot4WheelExample/MicrocontrollerSumobot4WheelExample.cpp
)

set (SIMULATOR_SOURCE_FILES
    ${SIMULATOR_ENGINE_FILES}
    ${SIMULATOR_APP_FILES}
)

add_library(bots2d STATIC
    ${SIMULATOR_SOURCE_FILES}
)

target_include_directories(bots2d PRIVATE testapp)
target_include_directories(bots2d PRIVATE testapp/scenes)

target_include_directories(bots2d PRIVATE include)
target_include_directories(bots2d PRIVATE include/app)
target_include_directories(bots2d PRIVATE include/app/objects)
target_include_directories(bots2d PRIVATE include/engine/core)
target_include_directories(bots2d PRIVATE include/engine/renderer)
target_include_directories(bots2d PRIVATE include/engine/transforms)
target_include_directories(bots2d PRIVATE include/engine/physics)
target_include_directories(bots2d PRIVATE include/engine/controllers)
target_include_directories(bots2d PRIVATE include/engine/scene)
target_include_directories(bots2d PRIVATE include/app/controllers)
target_include_directories(bots2d PRIVATE external/glfw/deps)
target_include_directories(bots2d PRIVATE external/stb)
target_include_directories(bots2d PRIVATE external/glm)
target_include_directories(bots2d PRIVATE external/imgui)

# Build box2d and glfw as static libs
add_subdirectory(external/glfw)
target_link_libraries(bots2d PRIVATE glfw)
# Path to Box2D src to only build Box2D (e.g. not testbed)
add_subdirectory(external/Box2D/src)
target_link_libraries(bots2d PRIVATE box2d)

# Make Dear ImGui use GLAD2
add_definitions( -DIMGUI_IMPL_OPENGL_LOADER_GLAD2 )

set_target_properties(bots2d PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)



if(MSVC)
  target_compile_options(bots2d PRIVATE /W4 /WX)
else()
  target_compile_options(bots2d PRIVATE -Wall -Wextra -pedantic -Werror)
  # Ignore compile flags to keep the external repo intact
  set_source_files_properties(src/engine/renderer/stb_image.cpp PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-unused-but-set-variable")
endif()
