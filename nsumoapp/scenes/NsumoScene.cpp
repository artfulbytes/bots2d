#include "NsumoScene.h"
#include "components/KeyboardController.h"
#include "robots/Sumobot.h"
#include "playgrounds/Dohyo.h"

namespace {

class SumobotController : public KeyboardController
{
public:
    enum class Drive { Stop, Forward, Backward, Left, Right };

    void setDrive(Drive drive) {
        const float maxVoltage = m_sumobot->getMotorMaxVoltage();
        float leftVoltage = maxVoltage;
        float rightVoltage = maxVoltage;
        switch (drive) {
            case Drive::Stop:
                leftVoltage = rightVoltage = 0.0f;
                break;
            case Drive::Forward:
                leftVoltage = rightVoltage = maxVoltage;
                break;
            case Drive::Backward:
                leftVoltage = rightVoltage = -maxVoltage;
                break;
            case Drive::Left:
                leftVoltage = -maxVoltage;
                rightVoltage = maxVoltage;
                break;
            case Drive::Right:
                leftVoltage = maxVoltage;
                rightVoltage = -maxVoltage;
                break;
        }
        *m_sumobot->getVoltageLine(Sumobot::WheelMotorIndex::FrontLeft) = leftVoltage;
        *m_sumobot->getVoltageLine(Sumobot::WheelMotorIndex::BackLeft) = leftVoltage;
        *m_sumobot->getVoltageLine(Sumobot::WheelMotorIndex::FrontRight) = rightVoltage;
        *m_sumobot->getVoltageLine(Sumobot::WheelMotorIndex::BackRight) = rightVoltage;
    }
    SumobotController(Sumobot *sumobot) : m_sumobot(sumobot)
    {
        assert(sumobot != nullptr);
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
    void onFixedUpdate(float stepTime) override
    {
        (void)stepTime;
    }
private:
    Sumobot *m_sumobot = nullptr;
};
}

void NsumoScene::onFixedUpdate()
{
}

NsumoScene::NsumoScene() :
    Scene("Nsumo against a keyboard controlled sumobot",
    PhysicsWorld::Gravity::TopView,
    (1/1000.0f))
{
    const Dohyo::Specification dohyoSpec =
    {
        0.35f,
        0.37f,
        Dohyo::TextureType::Scratched
    };
    m_dohyo = std::make_unique<Dohyo>(this, dohyoSpec, glm::vec2{ 0.0f, 0.0f });

    m_fourWheelBotOpponent = std::make_unique<Sumobot>(this,
            Sumobot::getBlueprintSpec(Sumobot::Blueprint::FourWheel),
            glm::vec2{-0.15f, 0.15f}, 4.71f);
    m_keyboardController =
        std::make_unique<SumobotController>(m_fourWheelBotOpponent.get());
    m_fourWheelBotOpponent->setController(m_keyboardController.get());
}

NsumoScene::~NsumoScene()
{
}
