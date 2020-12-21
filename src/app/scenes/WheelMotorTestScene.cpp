#include "WheelMotorTestScene.h"
#include "actuators/WheelMotor.h"
#include "components/Transforms.h"
#include "components/KeyboardController.h"

namespace {
    const float voltageToApply = 2.0f;

    class WheelMotorController : public KeyboardController
    {
    public:
        WheelMotorController(WheelMotor *wheelMotor) : m_wheelMotor(wheelMotor)
        {
            assert(wheelMotor != nullptr);
        }
        void onKeyEvent(const Event::Key &keyEvent) override
        {
            if (keyEvent.code == Event::KeyCode::Up) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_wheelMotor->getVoltageLine() = voltageToApply;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_wheelMotor->getVoltageLine() = 0.0f;
                }
            } else if (keyEvent.code == Event::KeyCode::Down) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_wheelMotor->getVoltageLine() = -voltageToApply;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_wheelMotor->getVoltageLine() = 0.0f;
                }
            }
        }
        void onFixedUpdate(double stepTime) override
        {
        }
    private:
        WheelMotor *m_wheelMotor = nullptr;
    };
}

WheelMotorTestScene::WheelMotorTestScene() :
    Scene(PhysicsWorld::Gravity::TopView)
{
    auto physicsWorld = m_physicsWorld.get();

    const WheelMotor::Specification unscaledSpec = {
        314.0f,
        89.0f,
        6.0f,
        0.12f,
        0.06f,
        0.05f
    };
    m_wheelMotor = std::make_unique<WheelMotor>(this, *physicsWorld, unscaledSpec, WheelMotor::Orientation::Left, glm::vec2{ 0.0f, 0.0f });
    m_wheelMotorController = std::make_unique<WheelMotorController>(m_wheelMotor.get());
    m_wheelMotor->setController(m_wheelMotorController.get());
}
