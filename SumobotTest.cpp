#include "SumobotTest.h"
#include "Sumobot4Wheel.h"
#include "WheelMotor.h"
#include "Dohyo.h"


SumobotTest::SumobotTest()
{
    //TODO: Where to initialize variables?
    m_world->SetGravity(b2Vec2(0,0));
    m_sumobot = new Sumobot4Wheel(m_world, 0.1f, 0.1f, 0.5f);
    /*
    m_testWheel = new Wheel(m_world, Wheel::Type::Left, 0.1f, 0.15f, 0.5f);
    m_testWheel->setCharacteristics(10.0f, -10.0f, 100.0f, 0);
    */
    m_dohyo = new Dohyo(m_world, 0.0f, 0.0f, 0.77f, 0.8f);

    m_keysPressed = 0;
}

void SumobotTest::Keyboard(int key)
{
    switch (key) {
    case 65: m_keysPressed |= static_cast<unsigned>(KeyPress::Left); break;
    case 68: m_keysPressed |= static_cast<unsigned>(KeyPress::Right); break;
    case 87: m_keysPressed |= static_cast<unsigned>(KeyPress::Up); break;
    case 83: m_keysPressed |= static_cast<unsigned>(KeyPress::Down); break;
    default: Test::Keyboard(key);
    }
}

void SumobotTest::KeyboardUp(int key)
{
    switch (key) {
    case 65: m_keysPressed &= ~static_cast<unsigned>(KeyPress::Left); break;
    case 68: m_keysPressed &= ~static_cast<unsigned>(KeyPress::Right); break;
    case 87: m_keysPressed &= ~static_cast<unsigned>(KeyPress::Up); break;
    case 83: m_keysPressed &= ~static_cast<unsigned>(KeyPress::Down); break;
    default: Test::Keyboard(key);
    }
}

void SumobotTest::stepTestWheel(Sumobot4Wheel::Drive drive)
{
    if (m_testWheel == nullptr) {
        return;
    }

    WheelMotor::Drive wDrive;
    switch (drive) {
        case Sumobot4Wheel::Drive::Forward:
            wDrive = WheelMotor::Drive::Forward;
            break;
        case Sumobot4Wheel::Drive::Backward:
            wDrive = WheelMotor::Drive::Backward;
            break;
        case Sumobot4Wheel::Drive::None:
            wDrive = WheelMotor::Drive::None;
            break;
    }
    m_testWheel->updateFrictionTest(wDrive);
}

void SumobotTest::Step(Settings& settings)
{
    Sumobot4Wheel::Drive drive = Sumobot4Wheel::Drive::None;
    Sumobot4Wheel::Turn turn = Sumobot4Wheel::Turn::None;

    if (m_keysPressed & static_cast<unsigned>(KeyPress::Right)) {
        turn = Sumobot4Wheel::Turn::Right;
    } else if (m_keysPressed & static_cast<unsigned>(KeyPress::Left)) {
        turn = Sumobot4Wheel::Turn::Left;
    }

    if (m_keysPressed & static_cast<unsigned>(KeyPress::Up)) {
        drive = Sumobot4Wheel::Drive::Forward;
    } else if (m_keysPressed & static_cast<unsigned>(KeyPress::Down)) {
        drive = Sumobot4Wheel::Drive::Backward;
    }

    m_sumobot->update(drive, turn);

    Test::Step(settings);
    g_debugDraw.DrawString(5, m_textLine, "Press w/a/s/d to control the sumobot");
    m_textLine += 15;
}



