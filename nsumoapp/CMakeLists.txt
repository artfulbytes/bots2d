cmake_minimum_required(VERSION 3.8)
project(nsumoapp VERSION 1.0)

# Modify to where the bots2d folder is stored
set (BOTS2D_RELATIVE_PATH ../)

set (NSUMOAPP_ASSETS
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/RectObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/QuadObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/CircleObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/shapes/LineObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/actuators/WheelMotor.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/bodies/SimpleBotBody.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/BaseBot.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/robots/Sumobot.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/sensors/RangeSensorObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/sensors/LineDetectorObject.cpp
    ${BOTS2D_RELATIVE_PATH}/assets/objects/playgrounds/Dohyo.cpp
)

set (NSUMOAPP_SCENES
    scenes/NsumoScene.cpp
)

set (NSUMOAPP_FILES
    ${NSUMOAPP_ASSETS}
    ${NSUMOAPP_SCENES}
    NsumoApp.cpp
    main.cpp
)

add_executable(nsumoapp
    ${NSUMOAPP_FILES}
)

target_include_directories(nsumoapp PRIVATE scenes)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/assets/objects)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/core)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/renderer)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/transforms)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/scene)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/physics)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/controllers)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/src/controllers/components)
target_include_directories(nsumoapp PRIVATE ${BOTS2D_RELATIVE_PATH}/external/glm)

# Include bots2d and build it as a static lib
add_subdirectory(${BOTS2D_RELATIVE_PATH} build)
target_link_libraries(nsumoapp PRIVATE bots2d)

target_compile_definitions(nsumoapp PRIVATE BUILD_TEST)

set_target_properties(nsumoapp PROPERTIES
       CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(nsumoapp PRIVATE /W4 /WX)
else()
  target_compile_options(nsumoapp PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
