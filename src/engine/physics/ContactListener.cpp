#include "ContactListener.h"
#include "Body2DUserData.h"

void ContactListener::updateLineDetector(b2Contact *contact, bool begin)
{
    b2BodyUserData bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    b2BodyUserData bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if (bodyAUserData.pointer && bodyBUserData.pointer) {
        Body2DUserData *userDataA = reinterpret_cast<Body2DUserData*>(bodyAUserData.pointer);
        Body2DUserData *userDataB = reinterpret_cast<Body2DUserData*>(bodyBUserData.pointer);

        if (userDataA->bodyId == BodyId::LineDetector && userDataB->bodyId == BodyId::Detectable) {
            userDataA->contactCount += begin ? 1 : -1;
        } else if (userDataB->bodyId == BodyId::LineDetector && userDataA->bodyId == BodyId::Detectable) {
            userDataB->contactCount += begin ? 1 : -1;
        }
    }
}

void ContactListener::BeginContact(b2Contact *contact) {
    updateLineDetector(contact, true);
}

void ContactListener::EndContact(b2Contact *contact) {
    updateLineDetector(contact, false);
}
