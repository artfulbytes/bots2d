#include "SumobotTestScene.h"
#include "components/Transforms.h"
#include "components/RectComponent.h"
#include "components/Body2D.h"
#include "components/KeyboardController.h"
#include "Sumobot4WheelExample/MicrocontrollerSumobot4WheelExample.h"
#include "robots/Sumobot.h"
#include "shapes/RectObject.h"
#include "playgrounds/Dohyo.h"


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
            //*m_sumobot->getVoltageLine(Sumobot::WheelMotorIndex::Left) = leftVoltage;
            //*m_sumobot->getVoltageLine(Sumobot::WheelMotorIndex::Right) = rightVoltage;
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
        void onFixedUpdate(double stepTime) override
        {
            (void)stepTime;
        }
    private:
        Sumobot *m_sumobot = nullptr;
    };
}

void SumobotTestScene::createBackground()
{
    const float backgroundWidth = 500.0f;
    const float backgroundHeight = backgroundWidth;
    glm::vec4 leftColor(0.906f, 0.294f, 0.235f, 1.0f);
    m_background->leftSide = std::make_unique<RectObject>(this, leftColor, nullptr, glm::vec2{ -backgroundWidth / 4.0f, 0.0f },
                                                          glm::vec2{ backgroundWidth / 2.0f, backgroundHeight }, 0.0f);
    glm::vec4 rightColor(0.153f, 0.565f, 0.69f, 1.0f);
    m_background->rightSide = std::make_unique<RectObject>(this, rightColor, nullptr, glm::vec2{ backgroundWidth / 4.0f, 0.0f },
                                                           glm::vec2{ backgroundWidth / 2.0f, backgroundHeight }, 0.0f);
}

SumobotTestScene::SumobotTestScene() :
    Scene("Test keyboard-controlled sumobot and dohyo@74cm", PhysicsWorld::Gravity::TopView)
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
                                               glm::vec2{0.5f, -0.5f}, 4.71f);
    m_fourWheelBot->setController(new SumobotController(m_fourWheelBot.get()));
    m_fourWheelBot->setDebug(true);
    m_twoWheelRectangleBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRectangle),
                                                       glm::vec2{-0.25f, 0.0f}, 1.5f);
    //m_twoWheelRectangleBot->setController(new SumobotController(m_twoWheelRectangleBot.get()));
    m_twoWheelRoundBlackBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRoundBlack),
                                                        glm::vec2{0.2f, -0.2f}, 3.0f);
    m_twoWheelRoundRedBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRoundRed),
                                                      glm::vec2{-0.15f, -0.15f}, 1.0f);
}

SumobotTestScene::~SumobotTestScene()
{
}
