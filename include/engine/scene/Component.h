#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "SceneObject.h"

struct Component
{
    SceneObject *m_parent = nullptr;
};

#endif /* COMPONENT_H_ */
