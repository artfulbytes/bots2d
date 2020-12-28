#ifndef WHEEL_MOTOR_H_
#define WHEEL_MOTOR_H_

#include "SceneObject.h"
#include <glm/glm.hpp>
#include <string>

class PhysicsWorld;
class SpriteAnimation;
class Body2D;

class WheelMotor : public SceneObject
{
public:
    enum class Orientation { Left, Right };
    enum class TextureType { None, Green, Orange, Red };
    struct Specification {
        Specification(float width, float diameter, float mass, TextureType textureType) :
            width(width), diameter(diameter), mass(mass), textureType(textureType) {}
        Specification(float voltageInConstant, float angularSpeedConstant, float maxVoltage,
                      float sidewayFrictionConstant, float width, float diameter, float mass,
                      TextureType textureType) :
            voltageInConstant(voltageInConstant), angularSpeedConstant(angularSpeedConstant),
            maxVoltage(maxVoltage), sidewayFrictionConstant(sidewayFrictionConstant),
            width(width), diameter(diameter), mass(mass), textureType(textureType) {}
        /* Simplified DC motor model
         * Tune voltageInConstant and angularSpeedConstant for different acceleration
         * and top speed (see tools/dc_motor_plot.py)
         * voltageInConstant = (Torque constant * Voltage constant) / Motor resistance */
        float voltageInConstant = 0.00628f;
        /* angularSpeedConstant = Torque constant / Motor resistance */
        float angularSpeedConstant = 0.00178f;
        /* Max voltage that can be applied to the voltage line */
        float maxVoltage = 6.0f;
        /* Made-up constant to tweak the sideway friction, larger value
         * means more friction and less skidding */
        float sidewayFrictionConstant = 100.0f;
        /* Negative value means use default */
        float torqueFrictionCoefficient = -1.0f;
        float width;
        float diameter;
        float mass;
        TextureType textureType = TextureType::None;
    };

    WheelMotor(Scene *scene, const Specification &spec, Orientation orientation,
               const glm::vec2 &startPosition, float startRotation);
    ~WheelMotor();

    void setVoltageIn(float voltage);
    void setDutyCycle(float dutyCycle);
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine();
    const Body2D *getBody() const { return m_body2D; }

private:
    void setAnimation();
    std::string getTextureName(Orientation orientation, TextureType textureType) const;
    void updateForce();

    Specification m_spec;
    float m_voltageIn = 0.0f;
    std::unique_ptr<SpriteAnimation> m_animation;
    Body2D *m_body2D = nullptr;
};

#endif /* WHEEL_MOTOR_H_ */
