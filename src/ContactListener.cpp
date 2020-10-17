#include "ContactListener.h"
#include "UserData.h"
#include <iostream>

void ContactListener::updateLineDetector(b2Contact* contact, bool begin)
{
    /* TODO: If user data is not a pointer anymore, this might not work... */
    b2BodyUserData* bodyAUserData = &contact->GetFixtureA()->GetBody()->GetUserData();
    b2BodyUserData* bodyBUserData = &contact->GetFixtureB()->GetBody()->GetUserData();
    if (bodyAUserData && bodyBUserData) {
        /* TODO: Before we did static cast here... */
        UserData* userDataA = reinterpret_cast<UserData*>(bodyAUserData);
        UserData* userDataB = reinterpret_cast<UserData*>(bodyBUserData);

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
