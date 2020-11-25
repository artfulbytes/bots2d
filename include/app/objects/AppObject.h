#ifndef APP_OBJECT_H_
#define APP_OBJECT_H_

#include "AppScene.h"

/* This is one abstraction level above SceneObject. AppObject can be seen as a container for
 * one or more  SceneObjects and can be the parent/child of other AppObjects. */
class AppObject {
public:
    /* Force app objects to be part of an app scene */
    AppObject(AppScene &appScene) { appScene.addObject(*this); };
    virtual ~AppObject() {};

    /* Called each update iteration before scene objects are updated.
     *
     * NOTE: If this object has a parent, this must be called explicitly
     *       by its parent. */
    virtual void onFixedUpdate(double stepTime) = 0;
    virtual void setParent(AppObject &parent) final { m_parent = &parent; };
    virtual const AppObject *getParent() final { return m_parent; };

protected:
    AppObject *m_parent = nullptr;
};

#endif /* APP_OBJECT_H_ */
