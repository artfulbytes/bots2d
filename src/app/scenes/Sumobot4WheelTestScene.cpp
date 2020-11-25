#include "Sumobot4WheelTestScene.h"
#include "TopViewSumobot4Wheel.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "Body2D.h"
#include "KeyboardController.h"
#include "sumobot4wheelexample/MicrocontrollerSumobot4WheelExample.h"

#include "TopViewDohyo.h"

namespace {
    class Sumobot4WheelController : public KeyboardController
    {
    public:
        enum class Drive { Stop, Forward, Backward, Left, Right };
        const float maxSpeed = 3.0f;

        void setDrive(Drive drive) {
            float leftSpeed = 0.0f;
            float rightSpeed = 0.0f;
            switch (drive) {
                case Drive::Stop:
                    leftSpeed = rightSpeed = 0.0f;
                    break;
                case Drive::Forward:
                    leftSpeed = rightSpeed = maxSpeed;
                    break;
                case Drive::Backward:
                    leftSpeed = rightSpeed = -maxSpeed;
                    break;
                case Drive::Left:
                    leftSpeed = -maxSpeed;
                    rightSpeed = maxSpeed;
                    break;
                case Drive::Right:
                    leftSpeed = maxSpeed;
                    rightSpeed = -maxSpeed;
                    break;
            }
            *m_sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontLeftMotor) = leftSpeed;
            *m_sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::BackLeftMotor) = leftSpeed;
            *m_sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontRightMotor) = rightSpeed;
            *m_sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::BackRightMotor) = rightSpeed;
        }
        Sumobot4WheelController(TopViewSumobot4Wheel &sumobot4Wheel) : m_sumobot4Wheel(&sumobot4Wheel) {}
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
        TopViewSumobot4Wheel *m_sumobot4Wheel = nullptr;
    };
}

Sumobot4WheelTestScene::Sumobot4WheelTestScene()
{
    PhysicsWorld *world = new PhysicsWorld(PhysicsWorld::Gravity::TopView);
    m_scene->setPhysicsWorld(world);

    TopViewDohyo *dohyo = new TopViewDohyo(*this, *world, { .innerRadius = 0.35f, .outerRadius = 0.37f }, Vec2(0, 0));

    glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);
    QuadTransform *transformBox = new QuadTransform();
    transformBox->position = { 0.2f, 0.2f, 0.0f };
    transformBox->size = { 0.07f, 0.07f };
    QuadComponent *boxComponent = new QuadComponent(color);
    auto boxBody = new Body2D(*world, *transformBox, true, true, 1.0f);
    m_scene->createObject(transformBox, boxComponent, boxBody, nullptr);

    TopViewSumobot4Wheel *sumobot4Wheel = new TopViewSumobot4Wheel(*this, *world, { .length = 0.1f, .width = 0.1f, .mass = 0.5f} , Vec2(0, 0));
    Sumobot4WheelController *controller = new Sumobot4WheelController(*sumobot4Wheel);
    m_scene->createObject(nullptr, nullptr, nullptr, controller);

    Microcontroller::VoltageLineArray voltageLines;
    voltageLines[Microcontroller::VOLTAGE_LINE_A0] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontLeftMotor);
    voltageLines[Microcontroller::VOLTAGE_LINE_A1] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::BackLeftMotor);
    voltageLines[Microcontroller::VOLTAGE_LINE_A2] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontRightMotor);
    voltageLines[Microcontroller::VOLTAGE_LINE_A3] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::BackRightMotor);
    voltageLines[Microcontroller::VOLTAGE_LINE_A4] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::LeftRangeSensor);
    voltageLines[Microcontroller::VOLTAGE_LINE_A5] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontLeftRangeSensor);
    voltageLines[Microcontroller::VOLTAGE_LINE_A6] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontRangeSensor);
    voltageLines[Microcontroller::VOLTAGE_LINE_A7] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontRightRangeSensor);
    voltageLines[Microcontroller::VOLTAGE_LINE_B0] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::RightRangeSensor);
    voltageLines[Microcontroller::VOLTAGE_LINE_B1] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontLeftLineDetector);
    voltageLines[Microcontroller::VOLTAGE_LINE_B2] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::BackLeftLineDetector);
    voltageLines[Microcontroller::VOLTAGE_LINE_B3] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::FrontRightLineDetector);
    voltageLines[Microcontroller::VOLTAGE_LINE_B4] = sumobot4Wheel->getVoltageLine(TopViewSumobot4Wheel::VoltageLine::BackRightLineDetector);
    auto controllerCBinding = new MicrocontrollerSumobot4WheelExample(voltageLines);

    m_scene->createObject(nullptr, nullptr, nullptr, controllerCBinding);
}
