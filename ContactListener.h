#ifndef CONTACT_LISTENER_H_
#define CONTACT_LISTENER_H_
#include <box2d/b2_contact.h>
#include <box2d/b2_world_callbacks.h>

class ContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);

private:
    void updateLineDetector(b2Contact* contact, bool begin);
};

#endif /* CONTACT_LISTENER_H_ */
