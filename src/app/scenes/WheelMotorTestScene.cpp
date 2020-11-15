#include "WheelMotorTestScene.h"
#include "TopViewWheelMotor.h"
#include "Transforms.h"
#include "KeyboardController.h"

namespace {
    class WheelMotorController : public KeyboardController
    {
    public:
        WheelMotorController(TopViewWheelMotor &wheelMotor) : m_wheelMotor(&wheelMotor) {}
        void onKeyEvent(const Event::Key &keyEvent) override
        {
            if (keyEvent.code == Event::KeyCode::Up) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_wheelMotor->getVoltageLine() = 3.0f;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_wheelMotor->getVoltageLine() = 0.0f;
                }
            } else if (keyEvent.code == Event::KeyCode::Down) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_wheelMotor->getVoltageLine() = -3.0f;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_wheelMotor->getVoltageLine() = 0.0f;
                }
            }
        }
        void onFixedUpdate(double stepTime) override
        {
        }
    private:
        TopViewWheelMotor *m_wheelMotor = nullptr;
    };
}

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
        .mass = 0.05f
    };
    TopViewWheelMotor *wheelMotor = new TopViewWheelMotor(*this, *world, spec, Vec2(0, 0));
    WheelMotorController *controller = new WheelMotorController(*wheelMotor);
    m_scene->createObject(nullptr, nullptr, nullptr, controller);
}
