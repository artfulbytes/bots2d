#include "SumobotTestScene.h"
#include "components/Transforms.h"
#include "components/RectComponent.h"
#include "components/Body2D.h"
#include "components/KeyboardController.h"
#include "NsumoController/NsumoMicrocontroller.h"
#include "robots/Sumobot.h"
#include "shapes/RectObject.h"
#include "playgrounds/Dohyo.h"
#include "ImGuiMenu.h"
#include <sstream>
#include <iomanip>
#include <iostream>


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

void SumobotTestScene::onFixedUpdate()
{
    /* Disable motors as they go out of the dohyo */
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

/* TODO: Break out into a separate class for better reuse? */
void SumobotTestScene::createTuningMenu()
{
    m_tuningMenu = std::make_unique<ImGuiMenu>(this, "Tuning menu", 250.0f, 15.0f, 400.0f, 300.0f);
    m_tuningMenu->addLabel("Sumobot 4W");
    auto fourWheelBot = m_fourWheelBot.get();
    m_tuningMenu->addSlider("Sideway fric. const.", 0.0f, 100.0f, fourWheelBot->getWheelSidewayFrictionConstant(),
        [fourWheelBot](float value){ fourWheelBot->setWheelSidewayFrictionConstant(value); }
    );
    m_tuningMenu->addSlider("Wheel friction coeff.", 0.0f, 1.0f, fourWheelBot->getWheelFrictionCoefficient(),
        [fourWheelBot](float value){ fourWheelBot->setWheelFrictionCoefficient(value); }
    );
    m_tuningMenu->addSlider("Wheel mass", 0.01f, 0.5f, fourWheelBot->getWheelMass(),
        [fourWheelBot](float value){ fourWheelBot->setWheelMass(value); }
    );
    m_tuningMenu->addSlider("Body mass", 0.1f, 1.0f, fourWheelBot->getBodyMass(),
        [fourWheelBot](float value){ fourWheelBot->setBodyMass(value); }
    );
    m_tuningMenu->addSlider("Max motor voltage", 1.0f, 12.0f, fourWheelBot->getMotorMaxVoltage(),
        [fourWheelBot](float value){ fourWheelBot->setMotorMaxVoltage(value); }
    );
    m_tuningMenu->addSlider("Motor voltage in constant", 0.001f, 0.06f, fourWheelBot->getMotorVoltageInConstant(),
        [fourWheelBot](float value){ fourWheelBot->setMotorVoltageInConstant(value); }
    );
    m_tuningMenu->addSlider("Motor angular speed constant", 0.0005f, 0.03f, fourWheelBot->getMotorAngularSpeedConstant(),
        [fourWheelBot](float value){ fourWheelBot->setMotorAngularSpeedConstant(value); }
    );
    m_tuningMenu->addSlider("Body angular damping", 0.0f, 500.0f, fourWheelBot->getAngularDamping(),
        [fourWheelBot](float value){ fourWheelBot->setAngularDamping(value); }
    );

    std::function<void(std::string)> setSpeedText;
    m_tuningMenu->addLabel("Speed: ", &setSpeedText);
    fourWheelBot->setForwardSpeedCallback([setSpeedText](float forwardSpeed){
        if (fabs(forwardSpeed) < 0.001f) {
            forwardSpeed = 0.0f;
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << forwardSpeed;
        std::string speedString = ss.str();
        setSpeedText("Speed: " + speedString + " m/s");
    });

    std::function<void(std::string)> setTopSpeedText;
    m_tuningMenu->addLabel("Top speed: ", &setTopSpeedText);
    fourWheelBot->setTopSpeedCallback([setTopSpeedText](float topSpeed){
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << topSpeed;
        std::string topSpeedString = ss.str();
        setTopSpeedText("Top speed: " + topSpeedString + " m/s");
    });

    std::function<void(std::string)> setAccelerationText;
    m_tuningMenu->addLabel("Acceleration: ", &setAccelerationText);
    fourWheelBot->setForwardAccelerationCallback([setAccelerationText](float forwardAcceleration){
        if (fabs(forwardAcceleration) < 0.001f) {
            forwardAcceleration = 0.0f;
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << forwardAcceleration;
        std::string accelerationString = ss.str();
        setAccelerationText("Acceleration: " + accelerationString + " m/s^2");
    });

    std::function<void(std::string)> setTimeToTopSpeedText;
    m_tuningMenu->addLabel("Top speed time: ", &setTimeToTopSpeedText);
    fourWheelBot->setTimeToTopSpeedCallback([setTimeToTopSpeedText](float timeToTopSpeed){
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << timeToTopSpeed;
        std::string timeToTopSpeedString = ss.str();
        setTimeToTopSpeedText("Top speed time: " + timeToTopSpeedString + " s");
    });

    std::function<void(std::string)> setTopSpeedAccelerationText;
    m_tuningMenu->addLabel("Top speed acc.: ", &setTopSpeedAccelerationText);
    fourWheelBot->setTopSpeedAccelerationCallback([setTopSpeedAccelerationText](float topSpeedAcceleration){
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << topSpeedAcceleration;
        std::string topSpeedAccelerationString = ss.str();
        setTopSpeedAccelerationText("Top speed acc.: " + topSpeedAccelerationString + " m/s^2");
    });

    std::function<void(std::string)> setTopAccelerationText;
    m_tuningMenu->addLabel("Top acceleration: ", &setTopAccelerationText);
    fourWheelBot->setTopAccelerationCallback([setTopAccelerationText](float topAcceleration){
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << topAcceleration;
        std::string topAccelerationString = ss.str();
        setTopAccelerationText("Top acceleration: " + topAccelerationString + " m/s^2");
    });

    std::function<void(std::string)> setTimeMovingText;
    m_tuningMenu->addLabel("Time moving: ", &setTimeMovingText);
    fourWheelBot->setTimeMovingCallback([setTimeMovingText](unsigned int timeMoving){
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << timeMoving;
        std::string timeMovedString = ss.str();
        setTimeMovingText("Time moving: " + timeMovedString + " ms");
    });
}

SumobotTestScene::SumobotTestScene() :
    Scene("Test different types of mini-class sumobots", PhysicsWorld::Gravity::TopView, (1/1000.0f))
{
    createBackground();

    const Dohyo::Specification dohyoSpec =
    {
        0.35f,
        0.37f,
        Dohyo::TextureType::Scratched
    };
    m_dohyo = std::make_unique<Dohyo>(this, dohyoSpec, glm::vec2{ 0.0f, 0.0f });

    m_fourWheelBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::Nsumo),
                                               glm::vec2{0.275f, 0.0f}, 1.57f);
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
    m_microcontroller = std::make_unique<NsumoMicrocontroller>(voltageLines);
    //m_fourWheelBot->setController(m_microcontroller.get());
    m_microcontroller->start();
    m_fourWheelBot->setDebug(true);
    m_keyboardController = std::make_unique<SumobotController>(m_fourWheelBot.get());
#if 0
    //m_fourWheelBot->setController(m_keyboardController.get());
    //m_fourWheelBotOpponent = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::FourWheel),
                                                       //glm::vec2{-0.15f, 0.15f}, 4.71f);
    //m_keyboardController = std::make_unique<SumobotController>(m_fourWheelBotOpponent.get());
    //m_fourWheelBotOpponent->setController(m_keyboardController.get());
#endif
    m_fourWheelBot->setController(m_keyboardController.get());

#if 0
    m_twoWheelRectangleBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRectangle),
                                                       glm::vec2{-0.25f, 0.0f}, 1.5f);
    m_twoWheelRoundBlackBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRoundBlack),
                                                        glm::vec2{0.2f, -0.2f}, 3.0f);
    m_twoWheelRoundRedBot = std::make_unique<Sumobot>(this, Sumobot::getBlueprintSpec(Sumobot::Blueprint::TwoWheelRoundRed),
                                                      glm::vec2{-0.15f, -0.15f}, 1.0f);
#endif
    createTuningMenu();
}

SumobotTestScene::~SumobotTestScene()
{
}
