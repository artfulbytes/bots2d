#include "NsumoScene.h"
#include "components/KeyboardController.h"
#include "robots/Sumobot.h"
#include "playgrounds/Dohyo.h"
#include "NsumoController/NsumoMicrocontroller.h"

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
    // Disable motors as they go out of the dohyo
    const auto frontLeftWheelPos = m_fourWheelBot->getAbsoluteWheelPosition(Sumobot::WheelMotorIndex::FrontLeft);
    const auto frontRightWheelPos = m_fourWheelBot->getAbsoluteWheelPosition(Sumobot::WheelMotorIndex::FrontRight);
    const auto backLeftWheelPos = m_fourWheelBot->getAbsoluteWheelPosition(Sumobot::WheelMotorIndex::BackLeft);
    const auto backRightWheelPos = m_fourWheelBot->getAbsoluteWheelPosition(Sumobot::WheelMotorIndex::BackRight);
    float outsideRadius = 0.38f;
    if (glm::length(frontLeftWheelPos) > outsideRadius) {
        m_fourWheelBot->disableMotor(Sumobot::WheelMotorIndex::FrontLeft);
    } else {
        m_fourWheelBot->enableMotor(Sumobot::WheelMotorIndex::FrontLeft);
    }
    if (glm::length(frontRightWheelPos) > outsideRadius) {
        m_fourWheelBot->disableMotor(Sumobot::WheelMotorIndex::FrontRight);
    } else {
        m_fourWheelBot->enableMotor(Sumobot::WheelMotorIndex::FrontRight);
    }
    if (glm::length(backLeftWheelPos) > outsideRadius) {
        m_fourWheelBot->disableMotor(Sumobot::WheelMotorIndex::BackLeft);
    } else {
        m_fourWheelBot->enableMotor(Sumobot::WheelMotorIndex::BackLeft);
    }
    if (glm::length(backRightWheelPos) > outsideRadius) {
        m_fourWheelBot->disableMotor(Sumobot::WheelMotorIndex::BackRight);
    } else {
        m_fourWheelBot->enableMotor(Sumobot::WheelMotorIndex::BackRight);
    }
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

    m_fourWheelBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::Nsumo),
                                           glm::vec2{0.31f, 0.0f}, 0.0f);
    m_fourWheelBot->setDebug(true);
    Microcontroller::VoltageLines voltageLines;
    // Assignment must match voltage_idx_e in voltage_lines.h
    voltageLines[Microcontroller::VoltageLine::A0] = { Microcontroller::VoltageLine::Type::Output, m_fourWheelBot->getVoltageLine(Sumobot::WheelMotorIndex::FrontLeft) };
    voltageLines[Microcontroller::VoltageLine::A1] = { Microcontroller::VoltageLine::Type::Output, m_fourWheelBot->getVoltageLine(Sumobot::WheelMotorIndex::BackLeft) };
    voltageLines[Microcontroller::VoltageLine::A2] = { Microcontroller::VoltageLine::Type::Output, m_fourWheelBot->getVoltageLine(Sumobot::WheelMotorIndex::FrontRight) };
    voltageLines[Microcontroller::VoltageLine::A3] = { Microcontroller::VoltageLine::Type::Output, m_fourWheelBot->getVoltageLine(Sumobot::WheelMotorIndex::BackRight) };
    voltageLines[Microcontroller::VoltageLine::A4] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::LineDetectorIndex::FrontLeft) };
    voltageLines[Microcontroller::VoltageLine::A5] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::LineDetectorIndex::BackLeft) };
    voltageLines[Microcontroller::VoltageLine::A6] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::LineDetectorIndex::FrontRight) };
    voltageLines[Microcontroller::VoltageLine::A7] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::LineDetectorIndex::BackRight) };
    voltageLines[Microcontroller::VoltageLine::B0] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::RangeSensorIndex::Left) };
    voltageLines[Microcontroller::VoltageLine::B1] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::RangeSensorIndex::FrontLeft) };
    voltageLines[Microcontroller::VoltageLine::B2] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::RangeSensorIndex::Front) };
    voltageLines[Microcontroller::VoltageLine::B3] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::RangeSensorIndex::FrontRight) };
    voltageLines[Microcontroller::VoltageLine::B4] = { Microcontroller::VoltageLine::Type::Input, m_fourWheelBot->getVoltageLine(Sumobot::RangeSensorIndex::Right) };
    m_microcontroller = std::make_unique<NsumoMicrocontroller>(voltageLines);
    m_fourWheelBot->setController(m_microcontroller.get());
    m_microcontroller->start();
}

NsumoScene::~NsumoScene()
{
}
