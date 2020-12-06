#ifndef VEC_2_H_
#define VEC_2_H_
#include <glm/glm.hpp>

template <class T>
struct Vec2 {
    Vec2(T x, T y) : x(x), y(y) {}
    T x;
    T y;
    inline float length() const {
        return glm::sqrt(x*x + y*y);
    }

    Vec2 operator+(const Vec2 &vec) const {
        return Vec2(this->x + vec.x, this->y + vec.y);
    }

    Vec2 operator*(T multiplier) {
        this->x *= multiplier;
        this->y *= multiplier;
        return *this;
    }

    void operator=(const Vec2 &vec) {
        this->x = vec.x;
        this->y = vec.y;
    }
};

#endif /* VEC_2_H_ */
