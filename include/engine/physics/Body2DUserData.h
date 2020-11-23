#ifndef BODY_2D_USER_DATA_H_
#define BODY_2D_USER_DATA_H_

#include <atomic>

enum class BodyId {LineDetector, DohyoBorder};

struct Body2DUserData
{
    std::atomic<int> contactCount;
    std::atomic<bool> inContact;
    BodyId bodyId;
};

#endif /* BODY_2D_USER_DATA_H_ */
