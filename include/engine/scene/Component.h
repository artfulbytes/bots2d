#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "SceneObject.h"

class Component
{
public:
    virtual ~Component() { }
    SceneObject *m_parent = nullptr;
};

#endif /* COMPONENT_H_ */
