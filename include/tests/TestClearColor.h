#ifndef TEST_CLEAR_COLOR_H_
#define TEST_CLEAR_COLOR_H_

#include "Test.h"

class TestClearColor : public Test
{
public:
    TestClearColor();
    ~TestClearColor();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
private:
    float m_clearColor[4];
};
#endif /* TEST_CLEAR_COLOR_H_ */
