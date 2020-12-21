#include "Sumobot4WheelTestScene.h"
#include "components/Transforms.h"
#include "components/QuadComponent.h"
#include "components/Body2D.h"
#include "components/KeyboardController.h"
#include "Sumobot4WheelExample/MicrocontrollerSumobot4WheelExample.h"
#include "robots/Sumobot4Wheel.h"
#include "shapes/QuadObject.h"
#include "playgrounds/Dohyo.h"

namespace {
    class Sumobot4WheelController : public KeyboardController
    {
    public:
        enum class Drive { Stop, Forward, Backward, Left, Right };
        const float maxVoltage = 4.0f;

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
                    leftVoltage = -maxVoltage;
                    rightVoltage = maxVoltage;
                    break;
                case Drive::Right:
                    leftVoltage = maxVoltage;
                    rightVoltage = -maxVoltage;
                    break;
            }
            *m_sumobot4Wheel->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontLeftMotor) = leftVoltage;
            *m_sumobot4Wheel->getVoltageLine(Sumobot4Wheel::VoltageLine::BackLeftMotor) = leftVoltage;
            *m_sumobot4Wheel->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontRightMotor) = rightVoltage;
            *m_sumobot4Wheel->getVoltageLine(Sumobot4Wheel::VoltageLine::BackRightMotor) = rightVoltage;
        }
        Sumobot4WheelController(Sumobot4Wheel *sumobot4Wheel) : m_sumobot4Wheel(sumobot4Wheel)
        {
            assert(sumobot4Wheel != nullptr);
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
        void onFixedUpdate(double stepTime) override
        {
        }
    private:
        Sumobot4Wheel *m_sumobot4Wheel = nullptr;
    };
}

void Sumobot4WheelTestScene::createBackground()
{
    const float backgroundWidth = 500.0f;
    const float backgroundHeight = backgroundWidth;
    glm::vec4 leftColor(0.906f, 0.294f, 0.235f, 1.0f);
    m_background->leftSide = std::make_unique<QuadObject>(this, leftColor, nullptr, glm::vec2{ -backgroundWidth / 4.0f, 0.0f },
                                                          glm::vec2{ backgroundWidth / 2.0f, backgroundHeight }, 0.0f);
    glm::vec4 rightColor(0.153f, 0.565f, 0.69f, 1.0f);
    m_background->rightSide = std::make_unique<QuadObject>(this, rightColor, nullptr, glm::vec2{ backgroundWidth / 4.0f, 0.0f },
                                                           glm::vec2{ backgroundWidth / 2.0f, backgroundHeight }, 0.0f);
}

Sumobot4WheelTestScene::Sumobot4WheelTestScene() :
    Scene(PhysicsWorld::Gravity::TopView)
{
    auto physicsWorld = m_physicsWorld.get();
    createBackground();
    const Dohyo::Specification dohyoSpec =
    {
        .innerRadius = 0.35f,
        .outerRadius = 0.37f,
        .textureType = Dohyo::TextureType::Scratched
    };
    m_dohyo = std::make_unique<Dohyo>(this, *physicsWorld, dohyoSpec, glm::vec2{ 0.0f, 0.0f });

    /* Sumobot controlled by keyboard */
    const Sumobot4Wheel::Specification circuitedSpec =
    {
        .length = 0.1f,
        .width = 0.1f,
        .mass = 0.5f,
        .textureType = Sumobot4Wheel::TextureType::Circuited,
    };
    m_sumobot4WheelCircuited = std::make_unique<Sumobot4Wheel>(this, *physicsWorld, circuitedSpec, glm::vec2(-0.25f, -0.25f));
    m_circuitedController = std::make_unique<Sumobot4WheelController>(m_sumobot4WheelCircuited.get());
    m_sumobot4WheelCircuited->setController(m_circuitedController.get());

    /* Sumobot controlled by microcontroller */
    const Sumobot4Wheel::Specification platedSpec =
    {
        .length = 0.1f,
        .width = 0.1f,
        .mass = 0.5f,
        .textureType = Sumobot4Wheel::TextureType::Plated,
    };
    m_sumobot4WheelPlated = std::make_unique<Sumobot4Wheel>(this, *physicsWorld, platedSpec, glm::vec2{ 0.0f, 0.0f });

    Microcontroller::VoltageLines voltageLines;
    voltageLines[Microcontroller::VoltageLine::Idx::A0] =
        { Microcontroller::VoltageLine::Type::Output,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontLeftMotor) };
    voltageLines[Microcontroller::VoltageLine::Idx::A1] =
        { Microcontroller::VoltageLine::Type::Output,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::BackLeftMotor) };
    voltageLines[Microcontroller::VoltageLine::Idx::A2] =
        { Microcontroller::VoltageLine::Type::Output,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontRightMotor) };
    voltageLines[Microcontroller::VoltageLine::Idx::A3] =
        { Microcontroller::VoltageLine::Type::Output,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::BackRightMotor) };
    voltageLines[Microcontroller::VoltageLine::Idx::A4] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::LeftRangeSensor) };
    voltageLines[Microcontroller::VoltageLine::Idx::A5] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontLeftRangeSensor) };
    voltageLines[Microcontroller::VoltageLine::Idx::A6] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontRangeSensor) };
    voltageLines[Microcontroller::VoltageLine::Idx::A7] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontRightRangeSensor) };
    voltageLines[Microcontroller::VoltageLine::Idx::B0] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::RightRangeSensor) };
    voltageLines[Microcontroller::VoltageLine::Idx::B1] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontLeftLineDetector) };
    voltageLines[Microcontroller::VoltageLine::Idx::B2] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::BackLeftLineDetector) };
    voltageLines[Microcontroller::VoltageLine::Idx::B3] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::FrontRightLineDetector) };
    voltageLines[Microcontroller::VoltageLine::Idx::B4] =
        { Microcontroller::VoltageLine::Type::Input,
          m_sumobot4WheelPlated->getVoltageLine(Sumobot4Wheel::VoltageLine::BackRightLineDetector) };
    m_platedController = std::make_unique<MicrocontrollerSumobot4WheelExample>(voltageLines);
    m_sumobot4WheelPlated->setController(m_platedController.get());
    auto controllerCBinding = static_cast<MicrocontrollerSumobot4WheelExample *>(m_platedController.get());
    controllerCBinding->start();
}

Sumobot4WheelTestScene::~Sumobot4WheelTestScene()
{
}
