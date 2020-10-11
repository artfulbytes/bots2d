#include "ContactListener.h"
#include "UserData.h"
#include <iostream>

void ContactListener::updateLineDetector(b2Contact* contact, bool begin)
{
    void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if (bodyAUserData && bodyBUserData) {
        UserData* userDataA = static_cast<UserData*>(bodyAUserData);
        UserData* userDataB = static_cast<UserData*>(bodyBUserData);

        if (userDataA->bodyType == BodyType::LineDetector
            && userDataB->bodyType == BodyType::DohyoBorder) {
            userDataA->contactCount += begin ? 1 : -1;
            //std::cout << "CONTACT!! " << userDataA->contactCount << "\n";
        } else if (userDataB->bodyType == BodyType::LineDetector
                   && userDataA->bodyType == BodyType::DohyoBorder) {
            userDataB->contactCount += begin ? 1 : -1;
            //std::cout << "END CONTACT!! " << userDataB->contactCount << "\n";
        }
        std::cout << std::flush;
        //userData->contactCount++;
        //userData->inContact = true;
    }
}

void ContactListener::BeginContact(b2Contact* contact) {
    updateLineDetector(contact, true);
}

void ContactListener::EndContact(b2Contact* contact) {
    updateLineDetector(contact, false);
}
