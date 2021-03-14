#include "SumobotTestScene.h"
#include "components/Transforms.h"
#include "components/RectComponent.h"
#include "components/Body2D.h"
#include "components/KeyboardController.h"
#include "Sumobot4WheelExample/MicrocontrollerSumobot4WheelExample.h"
#include "robots/Sumobot.h"
#include "shapes/RectObject.h"
#include "playgrounds/Dohyo.h"
#include "ImGuiMenu.h"

namespace {
class SumobotController : public KeyboardController
{
public:
    enum class Drive { Stop, Forward, Backward, Left, Right };
    const float maxVoltage = 6.0f;

    void setDrive(Drive drive) {
        float leftVoltage = 0.0f;
        float rightVoltage = 0.0f;
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
                leftVoltage = -maxVoltage * 0.6f;
                rightVoltage = maxVoltage * 0.6f;
                break;
            case Drive::Right:
                leftVoltage = maxVoltage * 0.6f;
                rightVoltage = -maxVoltage * 0.6f;
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
    void onFixedUpdate() override
    {
    }
private:
    Sumobot *m_sumobot = nullptr;
};
}

void SumobotTestScene::createBackground()
{
    const float backgroundWidth = 100.0f;
    const float backgroundHeight = backgroundWidth;
    const float middleStripeWidth = 0.075f;
    const glm::vec4 leftColor(0.906f, 0.294f, 0.235f, 1.0f);
    const glm::vec4 middleColor(0.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 rightColor(0.153f, 0.565f, 0.69f, 1.0f);
    m_background->leftSide = std::make_unique<RectObject>(this, leftColor, nullptr,
                                                          glm::vec2{ (-backgroundWidth / 4.0f) - (middleStripeWidth / 2.0f) , 0.0f },
                                                          glm::vec2{ backgroundWidth / 2.0f, backgroundHeight }, 0.0f);
    m_background->middleStripe = std::make_unique<RectObject>(this, middleColor, nullptr, glm::vec2{ 0.0f, 0.0f },
                                                              glm::vec2{ middleStripeWidth, backgroundHeight}, 0.0f);
    m_background->rightSide = std::make_unique<RectObject>(this, rightColor, nullptr,
                                                           glm::vec2{ (backgroundWidth / 4.0f) + (middleStripeWidth / 2.0f), 0.0f },
                                                           glm::vec2{ backgroundWidth / 2.0f, backgroundHeight }, 0.0f);
}

SumobotTestScene::SumobotTestScene() :
    Scene("Test different types of mini-class sumobots", PhysicsWorld::Gravity::TopView, (1/500.0f))
{
    createBackground();

    const Dohyo::Specification dohyoSpec =
    {
        0.35f,
        0.37f,
        Dohyo::TextureType::Scratched
    };
    m_dohyo = std::make_unique<Dohyo>(this, dohyoSpec, glm::vec2{ 0.0f, 0.0f });

    m_fourWheelBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::FourWheel),
                                               glm::vec2{0.15f, 0.15f}, 4.71f);
#if 0
    m_keyboardController = std::make_unique<SumobotController>(m_fourWheelBot.get());
    m_fourWheelBot->setController(m_keyboardController.get());
#endif
    m_fourWheelBotOpponent = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::FourWheel),
                                                       glm::vec2{-0.15f, 0.15f}, 4.71f);
    Microcontroller::VoltageLines voltageLines;
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
    m_microcontroller = std::make_unique<MicrocontrollerSumobot4WheelExample>(voltageLines, 500);
    m_fourWheelBot->setController(m_microcontroller.get());
    m_microcontroller->start();
    m_fourWheelBot->setDebug(true);
    m_twoWheelRectangleBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRectangle),
                                                       glm::vec2{-0.25f, 0.0f}, 1.5f);
    m_twoWheelRoundBlackBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRoundBlack),
                                                        glm::vec2{0.2f, -0.2f}, 3.0f);
    m_twoWheelRoundRedBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRoundRed),
                                                      glm::vec2{-0.15f, -0.15f}, 1.0f);
}

SumobotTestScene::~SumobotTestScene()
{
}
