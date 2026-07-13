
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "mind.h"
#include "objects.h"

class Entity : Object {
protected:
  double energy_{0.0};
  double power_of_engine_{0.0};
  double eye_power_{0.0};

public:
};
class EntityWithMind : Entity {
  Mind mind_;
};

#endif
