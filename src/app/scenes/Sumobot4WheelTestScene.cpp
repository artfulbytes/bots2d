#include "Sumobot4WheelTestScene.h"
#include "TopViewSumobot4Wheel.h"
#include "Transforms.h"
#include "QuadComponent.h"
#include "Body2D.h"
#include "KeyboardController.h"

#include "TopViewDohyo.h"

namespace {
    class Sumobot4WheelController : public KeyboardController
    {
    public:
        Sumobot4WheelController(TopViewSumobot4Wheel &sumobot4Wheel) : m_sumobot4Wheel(&sumobot4Wheel) {}
        void onKeyEvent(const Event::Key &keyEvent) override
        {
            if (keyEvent.code == Event::KeyCode::Up) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = 3.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = 3.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = 3.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = 3.0f;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = 0.0f;
                }
            } else if (keyEvent.code == Event::KeyCode::Down) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = -3.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = -3.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = -3.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = -3.0f;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = 0.0f;
                }
            } else if (keyEvent.code == Event::KeyCode::Left) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = -3.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = 3.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = -3.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = 3.0f;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = 0.0f;
                }
            } else if (keyEvent.code == Event::KeyCode::Right) {
                if (keyEvent.action == Event::KeyAction::Press) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = 3.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = -3.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = 3.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = -3.0f;
                } else if (keyEvent.action == Event::KeyAction::Release) {
                    *m_sumobot4Wheel->getFrontLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getFrontRightMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackLeftMotorVoltageLine() = 0.0f;
                    *m_sumobot4Wheel->getBackRightWheelVoltageLine() = 0.0f;
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
}
