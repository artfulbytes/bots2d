#ifndef TEST_DRAW_BASIC_SHAPES_H_
#define TEST_DRAW_BASIC_SHAPES_H_

#include "Test.h"

class TestDrawBasicShapes : public Test
{
public:
    TestDrawBasicShapes();
    ~TestDrawBasicShapes();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
private:
};

#endif /* TEST_DRAW_BASIC_SHAPES_H_ */
