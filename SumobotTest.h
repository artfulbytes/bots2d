#include "test.h"
#include "Sumobot4Wheel.h"


// TODO: Include in header or in CPP file?
class SumobotTest : public Test
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
    // TODO: Do you define overridden methods in header?
    void Keyboard(int key) override;
    void KeyboardUp(int key) override;
    void Step(Settings& settings) override;
    static Test* Create()
    {
	    return new SumobotTest;
    }
private:
    void stepTestWheel(Sumobot4Wheel::Drive drive);

    unsigned int m_keysPressed;
    Sumobot4Wheel* m_sumobot = nullptr;
    WheelMotor* m_testWheel = nullptr;
};

static int testIndex = RegisterTest("Benchmark", "Sumobot4Wheel", SumobotTest::Create);
