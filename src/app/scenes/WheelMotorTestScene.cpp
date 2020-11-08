#include "WheelMotorTestScene.h"
#include "TopViewWheelMotor.h"

WheelMotorTestScene::WheelMotorTestScene()
{
    PhysicsWorld *world = new PhysicsWorld(PhysicsWorld::Gravity::TopView);
    m_scene->setPhysicsWorld(world);

    TopViewWheelMotor::Specification spec = {
        .voltageInConstant = 314.0f,
        .angularSpeedConstant = 89.0f,
        .maxVoltage = 6.0f,
        .diameter = 0.03f,
        .width = 0.015f,
        .mass = 0.5f
    };
    auto topViewWheelMotor = new TopViewWheelMotor(*this, *world, spec, Vec2(0, 0));
    topViewWheelMotor->setVoltageIn(6.0f);
}
