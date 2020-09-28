#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace constants
{
    // TODO: Fix inline variables warning


    /* 
     * Box2D uses metric units and recommends sizing objects
     * between 0.1-10 m. In this simulation tool we want
     * sizes as small as 0.01 m. Therefore, introduce
     * a scale factor of 10, which converts 1 meter in Box2D
     * 10 cm, giving us a range between 0.01-1 m for our objects.
     */
    inline constexpr float physScaleFactor { 10.0f };
    inline constexpr float lengthScaleFactor { physScaleFactor };
    inline constexpr float speedScaleFactor { lengthScaleFactor };
    inline constexpr float accelerationFactor { speedScaleFactor };
    inline constexpr float massScaleFactor { physScaleFactor * physScaleFactor * physScaleFactor };
    /* F = m*a so F_scaled = forceFactor * F = (massFactor * m) * (accFactor * a) */
    /* TODO: DOUBLE CHECK */
    inline constexpr float forceScaleFactor { massScaleFactor * accelerationFactor };
    inline constexpr float maxWidthObject { 1.0f };
    inline constexpr float maxHeightObject { 0.01f };

    /*
     * TODO: FIX
     */
    inline constexpr float pixelScaleFactor { 1.0f };

    inline constexpr float degToGrad { 0.0174532925199432957f };
    inline constexpr float radToDeg { 57.295779513082320876f };
}

#endif /* CONSTANTS_H_ */
