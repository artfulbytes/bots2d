cmake_minimum_required(VERSION 3.8)
project(bots2dtest VERSION 1.0)

# Modify to where the bots2d folder is stored
set (BOTS2D_RELATIVE_PATH ../)

set (BOTS2D_ASSETS
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/RectObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/QuadObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/CircleObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/LineObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/actuators/WheelMotor.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/bodies/SimpleBotBody.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/BaseBot.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/PhysicsBot.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/Sumobot.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/LineFollower.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/sensors/RangeSensorObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/sensors/LineDetectorObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/playgrounds/Dohyo.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/playgrounds/LineFollowerPath.cpp
)

set (BOTS2D_TEST_SCENES
    scenes/DrawTestScene.cpp
    scenes/SpriteAnimationTestScene.cpp
    scenes/PhysicsTestScene.cpp
    scenes/WheelMotorTestScene.cpp
    scenes/PhysicsBotTestScene.cpp
    scenes/SumobotTestScene.cpp
    scenes/LineFollowerTestScene.cpp
)

set (BOTS2D_TEST_CONTROLLERS
    controllers/NsumoController/main.c
    controllers/NsumoController/NsumoMicrocontroller.cpp
    controllers/NsumoController/motor.c
    controllers/NsumoController/nsumo/drive.c
    controllers/NsumoController/nsumo/line_detection.c
    controllers/NsumoController/nsumo/state_machine.c
    controllers/NsumoController/nsumo/enemy_detection.c
    controllers/NsumoController/nsumo/trace.c
    controllers/NsumoController/nsumo/test.c
)

set (BOTS2D_TEST_FILES
    ${BOTS2D_ASSETS}
    ${BOTS2D_TEST_SCENES}
    ${BOTS2D_TEST_CONTROLLERS}
    Bots2DTestApp.cpp
    main.cpp
)

add_executable(bots2dtest
    ${BOTS2D_TEST_FILES}
)

target_include_directories(bots2dtest PRIVATE scenes)
target_include_directories(bots2dtest PRIVATE controllers)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/assets/objects)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/src/core)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/src/renderer)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/src/transforms)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/src/scene)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/src/physics)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/src/controllers)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/src/controllers/components)
target_include_directories(bots2dtest PRIVATE ${BOTS2D_RELATIVE_PATH}/external/glm)

# Include bots2d and build it as a static lib
add_subdirectory(${BOTS2D_RELATIVE_PATH} build)
target_link_libraries(bots2dtest PRIVATE bots2d)

set_target_properties(bots2dtest PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(bots2dtest PRIVATE /W4 /WX)
else()
  target_compile_options(bots2dtest PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
