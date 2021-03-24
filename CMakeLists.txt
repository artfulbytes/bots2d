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
    src/renderer/Renderer.cpp
    src/renderer/Camera.cpp
    src/renderer/Scalebar.cpp
    src/renderer/GLError.cpp
    src/renderer/AssetsHelper.cpp
    src/renderer/VertexBuffer.cpp
    src/renderer/IndexBuffer.cpp
    src/renderer/VertexArray.cpp
    src/renderer/Shader.cpp
    src/renderer/ImGuiOverlay.cpp
    src/renderer/Texture.cpp
    src/renderer/components/RectComponent.cpp
    src/renderer/components/QuadComponent.cpp
    src/renderer/SpriteAnimation.cpp
    src/renderer/stb_image.cpp
    src/renderer/ImGuiMenu.cpp
)

set (PHYSICS_SOURCE_FILES
    src/physics/PhysicsWorld.cpp
    src/physics/ContactListener.cpp
    src/physics/components/Body2D.cpp
    src/physics/components/RangeSensor.cpp
    src/physics/components/LineDetector.cpp
)

set (CONTROLLER_SOURCE_FILES
    src/controllers/components/Microcontroller.cpp
    src/controllers/components/CMicrocontroller.cpp
    src/controllers/components/microcontroller_c_bindings.c
)

set (BOTS2D_FILES
    src/core/Application.cpp
    src/core/Event.cpp
    src/scene/SceneObject.cpp
    src/scene/Scene.cpp
    src/scene/SceneMenu.cpp
    ${RENDERER_SOURCE_FILES}
    ${PHYSICS_SOURCE_FILES}
    ${CONTROLLER_SOURCE_FILES}
    ${GLAD_GL_SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
)

add_library(bots2d STATIC
    ${BOTS2D_FILES}
)

target_include_directories(bots2d PRIVATE src)
target_include_directories(bots2d PRIVATE src/core)
target_include_directories(bots2d PRIVATE src/renderer)
target_include_directories(bots2d PRIVATE src/transforms)
target_include_directories(bots2d PRIVATE src/physics)
target_include_directories(bots2d PRIVATE src/controllers)
target_include_directories(bots2d PRIVATE src/scene)
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
  set_source_files_properties(src/renderer/stb_image.cpp PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-unused-but-set-variable")
endif()
