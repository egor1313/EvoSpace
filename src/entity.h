
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "mind.h"
#include "object.h"

class Entity : Object {
protected:
  float energy_{0.0};
  float power_of_engine_{0.0};
  float eye_power_{0.0};

public:
};
class EntityWithMind : Entity {
  Mind mind_;
};

#endif
