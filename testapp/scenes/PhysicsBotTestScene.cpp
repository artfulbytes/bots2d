#include "PhysicsBotTestScene.h"
#include "components/KeyboardController.h"
#include "robots/PhysicsBot.h"


namespace {
class PhysicsBotController : public KeyboardController
{
public:
    enum class Drive { Stop, Forward, Backward, Left, Right };
    const float force = 6.0f;

    void setDrive(Drive drive) {
        float leftForce = 0.0f;
        float rightForce = 0.0f;
        switch (drive) {
            case Drive::Stop:
                leftForce = rightForce = 0.0f;
                break;
            case Drive::Forward:
                leftForce = rightForce = force;
                break;
            case Drive::Backward:
                leftForce = rightForce = -force;
                break;
            case Drive::Left:
                leftForce = -force;
                rightForce = force;
                break;
            case Drive::Right:
                leftForce = force;
                rightForce = -force;
                break;
        }

        m_physicsBot->setVoltageFrontLeft(leftForce);
        m_physicsBot->setVoltageBackLeft(leftForce);
        m_physicsBot->setVoltageFrontRight(rightForce);
        m_physicsBot->setVoltageBackRight(rightForce);
    }
    PhysicsBotController(PhysicsBot *physicsBot) : m_physicsBot(physicsBot)
    {
        assert(physicsBot != nullptr);
    }
    void onKeyEvent(const Event::Key &keyEvent) override
    {
        if (keyEvent.code == Event::KeyCode::Up) {
            if (keyEvent.action == Event::KeyAction::Press) {
                setDrive(Drive::Forward);
            } else if (keyEvent.action == Event::KeyAction::Release) {
                setDrive(Drive::Stop);
            }
        } else if (keyEvent.code == Event::KeyCode::Down) {
            if (keyEvent.action == Event::KeyAction::Press) {
                setDrive(Drive::Backward);
            } else if (keyEvent.action == Event::KeyAction::Release) {
                setDrive(Drive::Stop);
            }
        } else if (keyEvent.code == Event::KeyCode::Left) {
            if (keyEvent.action == Event::KeyAction::Press) {
                setDrive(Drive::Left);
            } else if (keyEvent.action == Event::KeyAction::Release) {
                setDrive(Drive::Stop);
            }
        } else if (keyEvent.code == Event::KeyCode::Right) {
            if (keyEvent.action == Event::KeyAction::Press) {
                setDrive(Drive::Right);
            } else if (keyEvent.action == Event::KeyAction::Release) {
                setDrive(Drive::Stop);
            }
        }
    }
    void onFixedUpdate() override
    {
    }
private:
    PhysicsBot *m_physicsBot = nullptr;
};
}

PhysicsBotTestScene::PhysicsBotTestScene() :
    Scene("Scene for experimenting with vehicle physics", PhysicsWorld::Gravity::TopView)
{
    m_physicsBot = std::make_unique<PhysicsBot>(this, glm::vec2{0.1f,0.1f}, glm::vec2{0,0}, 0);
    m_keyboardController = std::make_unique<PhysicsBotController>(m_physicsBot.get());
    m_physicsBot->setController(m_keyboardController.get());
}

PhysicsBotTestScene::~PhysicsBotTestScene()
{
}
