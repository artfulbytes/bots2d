#ifndef WHEEL_MOTOR_H_
#define WHEEL_MOTOR_H_

#include "SceneObject.h"
#include <glm/glm.hpp>
#include <string>

class PhysicsWorld;
class SpriteAnimation;
class Body2D;

/* WheelMotor simulates a wheel connected to a DC-motor in a top-view
 * gravity physics world. */
class WheelMotor : public SceneObject
{
public:
    enum class Orientation { Left, Right };
    enum class TextureType { None, Green, Orange };
    struct Specification {
        Specification(float width, float diameter, float mass, TextureType textureType) :
            width(width), diameter(diameter), mass(mass), textureType(textureType) {}
        Specification(float voltageInConstant, float angularSpeedConstant, float maxVoltage,
                      float maxLateralCancelingImpulse, float width, float diameter, float mass,
                      TextureType textureType) :
            voltageInConstant(voltageInConstant), angularSpeedConstant(angularSpeedConstant),
            maxVoltage(maxVoltage), maxLateralCancelingImpulse(maxLateralCancelingImpulse),
            width(width), diameter(diameter), mass(mass), textureType(textureType) {}
        /* Simplified DC motor model
         * Tune these two constants to get different characteristics
         * (Torque constant * Voltage constant) / Motor resistance */
        float voltageInConstant = 314.0f;
        /* Torque constant / Motor resistance */
        float angularSpeedConstant = 89.0f;
        /* Max voltage that can be applied to the voltage line */
        float maxVoltage = 6.0f;
        /* This determines the sideway friction */
        float maxLateralCancelingImpulse = 40.5f;
        float width;
        float diameter;
        float mass;
        TextureType textureType = TextureType::None;
    };

    WheelMotor(Scene *scene, const Specification &unscaledSpec,
               Orientation orientation, const glm::vec2 &unscaledStartPos = glm::vec2{0.0f, 0.0f});
    ~WheelMotor();

    void setVoltageIn(float voltage);
    void setDutyCycle(float dutyCycle);
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine();
    const Body2D *getBody() const { return m_body2D; }

private:
    static Specification scaleSpec(const Specification &unscaledSpec);
    void setAnimation();
    std::string getTextureName(Orientation orientation, TextureType textureType);
    void updateForce();

    Specification m_scaledSpec;
    float m_voltageIn = 0.0f;
    std::unique_ptr<SpriteAnimation> m_animation;
    Body2D *m_body2D = nullptr;
};

#endif /* WHEEL_MOTOR_H_ */
