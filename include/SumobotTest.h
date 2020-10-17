#include "Sumobot4Wheel.h"
#include "Dohyo.h"
#include "BoxObstacle.h"

class SumobotTest
{
public:
    enum class KeyPress : unsigned
    {
        Up = 0x01,
        Down = 0x02,
        Left = 0x04,
        Right = 0x08
    };
    SumobotTest();
    void Keyboard(int key);
    void KeyboardUp(int key);
    void Step();
private:
    void stepTestWheel(Sumobot4Wheel::Drive drive);

    unsigned int m_keysPressed;
    b2World *m_world = nullptr; /* TODO: How to create ? */
    Sumobot4Wheel* m_sumobot = nullptr;
    WheelMotor* m_testWheel = nullptr;
    Dohyo* m_dohyo = nullptr;
    BoxObstacle* m_boxObstacle = nullptr;
};

