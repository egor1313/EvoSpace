
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "config.hpp"
#include "genome.h"
#include "mind.h"
#include "planet.h"

class Entity : public Object {
private:
  Mind mind_{};
  Genome gen_{};
  double angle_{Config::Balance::Get().kStandartAngle};
  float energy_{Config::Balance::Get().kStandartEnergy};
  int life_epoch_{0};

public:
  Entity() = default;
  Entity(Genome &gen);
  bool isAlive() const;
  Genome &get_genome() const;
  Entity get_birth(const Entity &second_parrent) const;
  float get_energy() const;
  void mutate(float force = Config::Balance::Get().kStandartMutationForce);
  void do_impulse(double force);
  void rotate(float force);
  void atack_entity(const Entity &attacker);
  void get_atack(const Entity &victim);
  friend Planet &do_merge(const std::vector<Entity> &merge_list);
};

#endif
