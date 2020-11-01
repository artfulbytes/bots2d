#ifndef USER_DATA_H_
#define USER_DATA_H_

#include <atomic>

enum class BodyType {LineDetector, DohyoBorder};

struct UserData
{
    std::atomic<int> contactCount;
    std::atomic<bool> inContact;
    BodyType bodyType;
};

#endif /* USER_DATA_H_ */
