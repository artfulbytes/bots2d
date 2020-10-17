cmake_minimum_required(VERSION 3.8)
project(sumobot_simulator VERSION 1.0)

set (SIMULATOR_SOURCE_FILES
    src/main.cpp
    src/SumobotTest.cpp
    src/Sumobot4Wheel.cpp
    src/WheelMotor.cpp
    src/Dohyo.cpp
    src/ContactListener.cpp
    src/RangeSensor.cpp
    src/LineDetector.cpp
    src/BoxObstacle.cpp
)

# Path to Box2D src to only build Box2D (not unnecessary stuff like testbed)
add_subdirectory(external/Box2D/src)
add_executable(sumobot_simulator ${SIMULATOR_SOURCE_FILES})

target_include_directories(sumobot_simulator PRIVATE include)
target_link_libraries(sumobot_simulator PRIVATE box2d)
set_target_properties(sumobot_simulator PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
