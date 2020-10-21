#ifndef TEST_H_
#define TEST_H_

#include <vector>
#include <functional>
#include <string>
#include <iostream>

class Test
{
public:
    Test() {}
    virtual ~Test() {}

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
};

class TestMenu : public Test
{
public:
    TestMenu(Test*& currentTestPointer);

    void OnImGuiRender() override;

    template<typename T>
    void RegisterTest(const std::string& name)
    {
        std::cout << "Registering test " << name << std::endl;
        m_tests.push_back(std::make_pair(name, []() { return new T(); }));
    }
private:
    Test*& m_currentTest;
    std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;
};

#endif /* TEST_H_ */
