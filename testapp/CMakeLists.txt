cmake_minimum_required(VERSION 3.8)
project(bots2d_testapp VERSION 1.0)

set (SIMULATOR_TEST_APP_FILES
    main.cpp
    SimulatorTestApp.cpp
    scenes/DrawTestScene.cpp
    scenes/SpriteAnimationTestScene.cpp
    scenes/PhysicsTestScene.cpp
    scenes/WheelMotorTestScene.cpp
    scenes/SumobotTestScene.cpp
    scenes/LineFollowerTestScene.cpp
)

add_executable(bots2d_testapp
    ${SIMULATOR_TEST_APP_FILES}
)

target_include_directories(bots2d_testapp PRIVATE scenes)
target_include_directories(bots2d_testapp PRIVATE ../include/engine/core)
target_include_directories(bots2d_testapp PRIVATE ../include/engine/renderer)
target_include_directories(bots2d_testapp PRIVATE ../include/engine/transforms)
target_include_directories(bots2d_testapp PRIVATE ../include/engine/scene)
target_include_directories(bots2d_testapp PRIVATE ../include/engine/physics)
target_include_directories(bots2d_testapp PRIVATE ../include/engine/controllers)
target_include_directories(bots2d_testapp PRIVATE ../include/app/objects)
target_include_directories(bots2d_testapp PRIVATE ../include/app/controllers)

# Include bots2d and build it as a static lib
add_subdirectory(../ build)
target_link_libraries(bots2d_testapp PRIVATE bots2d)

set_target_properties(bots2d_testapp PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(bots2d_testapp PRIVATE /W4 /WX)
else()
  target_compile_options(bots2d_testapp PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
