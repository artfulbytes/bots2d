#include "LineFollowerTestScene.h"
#include "components/KeyboardController.h"
#include "robots/LineFollower.h"
#include "shapes/RectObject.h"
#include "playgrounds/LineFollowerPath.h"

namespace {
    class LineFollowerController : public KeyboardController
    {
    public:
        enum class Drive { Stop, Forward, Backward, Left, Right };
        const float maxVoltage = 3.0f;

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

            *m_lineFollower->getVoltageLine(LineFollower::WheelMotorIndex::Left) = leftVoltage;
            *m_lineFollower->getVoltageLine(LineFollower::WheelMotorIndex::Right) = rightVoltage;
        }
        LineFollowerController(LineFollower *lineFollower) : m_lineFollower(lineFollower)
        {
            assert(lineFollower != nullptr);
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
        LineFollower *m_lineFollower = nullptr;
    };
}

LineFollowerTestScene::LineFollowerTestScene() :
    Scene("Test keyboard-controlled line follower", PhysicsWorld::Gravity::TopView)
{
    const float bgWidth = 3.0f;
    const float bgHeight = bgWidth;
    const glm::vec4 bgColor(1.0f, 1.0f, 1.0f, 1.0f);
    const glm::vec4 lineColor(0.0f, 0.0f, 0.0f, 1.0f);
    const float lineWidth = 0.01f;
    m_background = std::make_unique<RectObject>(this, bgColor, nullptr, glm::vec2{ 0.0f, 0.0f },
                                                glm::vec2{ bgWidth, bgHeight }, 0.0f);

    m_lineFollowerPath = std::make_unique<LineFollowerPath>(this, lineColor, lineWidth,
                                                            LineFollowerPath::getBlueprintPathPoints(LineFollowerPath::Blueprint::Mshaped));
    m_lineFollower = std::make_unique<LineFollower>(this, LineFollower::getBlueprintSpec(LineFollower::Blueprint::FourFrontSensors),
                                               glm::vec2{-0.25f, 0.0f}, 1.5f);
    m_lineFollower->setController(new LineFollowerController(m_lineFollower.get()));
    m_lineFollower->setDebug(true);
}

LineFollowerTestScene::~LineFollowerTestScene()
{
}
