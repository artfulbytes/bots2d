#ifndef WHEEL_MOTOR_TEST_SCENE_H_
#define WHEEL_MOTOR_TEST_SCENE_H_

#include "Scene.h"

class WheelMotor;
class ControllerComponent;

class WheelMotorTestScene : public Scene
{
public:
    WheelMotorTestScene();

private:
    std::unique_ptr<WheelMotor> m_wheelMotor;
    std::unique_ptr<ControllerComponent> m_wheelMotorController;
};

#endif /* WHEEL_MOTOR_TEST_SCENE_H_ */
