#include "PhysicsBotTestScene.h"
#include "components/KeyboardController.h"
#include "robots/PhysicsBot.h"
#include "ImGuiMenu.h"
#include <sstream>
#include <iomanip>

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
    m_tuningMenu = std::make_unique<ImGuiMenu>(this, "Tuning menu", 250.0f, 15.0f, 400.0f, 300.0f);
    auto physicsBot = m_physicsBot.get();
    m_tuningMenu->addLabel("Friction");
    m_tuningMenu->addSlider("Sideway fric. const.", 0.0f, 1000.0f, physicsBot->getSidewayFrictionConstant(),
        [physicsBot](float value){ physicsBot->setSidewayFrictionConstant(value); }
    );
    m_tuningMenu->addSlider("Wheel friction coeff.", 0.0f, 1.0f, physicsBot->getFrictionCoefficient(),
        [physicsBot](float value){ physicsBot->setFrictionCoefficient(value); }
    );
    m_tuningMenu->addLabel("Mass");
    m_tuningMenu->addSlider("Physics bot", 0.01f, 10.0f, physicsBot->getTotalMass(),
        [physicsBot](float value){ physicsBot->setTotalMass(value); }
    );

    m_tuningMenu->addLabel("");

    std::function<void(std::string)> setSpeedText;
    m_tuningMenu->addLabel("Speed: ", &setSpeedText);
    physicsBot->setForwardSpeedCallback([setSpeedText](float forwardSpeed){
        if (fabs(forwardSpeed) < 0.001f) {
            forwardSpeed = 0.0f;
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << forwardSpeed;
        std::string speedString = ss.str();
        setSpeedText("Speed: " + speedString + " m/s");
    });

    std::function<void(std::string)> setAccelerationText;
    m_tuningMenu->addLabel("Acceleration: ", &setAccelerationText);
    physicsBot->setForwardAccelerationCallback([setAccelerationText](float forwardAcceleration){
        if (fabs(forwardAcceleration) < 0.001f) {
            forwardAcceleration = 0.0f;
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << forwardAcceleration;
        std::string accelerationString = ss.str();
        setAccelerationText("Acceleration: " + accelerationString + " m/s^2");
    });

    std::function<void(std::string)> setTopSpeedText;
    m_tuningMenu->addLabel("Top speed: ", &setTopSpeedText);
    physicsBot->setTopSpeedCallback([setTopSpeedText](float topSpeed){
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << topSpeed;
        std::string topSpeedString = ss.str();
        setTopSpeedText("Top speed: " + topSpeedString + " m/s");
    });

    std::function<void(std::string)> setAccToTopSpeedText;
    m_tuningMenu->addLabel("Acc. to top speed: ", &setAccToTopSpeedText);
    physicsBot->setAccelerationToTopSpeedCallback([setAccToTopSpeedText](float accToTopSpeed){
        if (accToTopSpeed > 1000000.0f) {
            accToTopSpeed = 0.0f;
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << accToTopSpeed;
        std::string accToTopSpeedString = ss.str();
        setAccToTopSpeedText("Acc. top speed: " + accToTopSpeedString + " m/s^2");
    });

    m_tuningMenu->addButton("Reset recorded values", [physicsBot](){
        physicsBot->resetRecordedValues();
    });

}

PhysicsBotTestScene::~PhysicsBotTestScene()
{
}
