#include "entity.h"

Entity::Entity(Genome &gen) : mind_{gen}, gen_{gen} {}
bool Entity::isAlive() const {
  if (life_epoch_ >= Config::Balance::Get().kStandartLifeTime || energy_ < 0) {
    return true;
  }
  return false;
}
float Entity::get_energy() const { return energy_; }
Genome &Entity::get_genome() const { return gen_; }
Entity Entity::get_birth(const Entity &second_parrent) const {
  /*if (energy_ < Config::Balance::Get().kStandartBirthNeedEnergy ||
      second_parrent.get_energy() <
          Config::Balance::Get().kStandartBirthNeedEnergy) {
    return;
  }*/

  // сначало без налога! Налог таков: Минус энергия и минус импульс (чуть чуть)

  Entity child{gen_.crossover(second_parrent.gen_)};
  child.mutate();
  return child;
}

void Entity::mutate(float force) {}

void Entity::do_impulse(double force) {
  force = gen_.get_params().power_of_engine;
}
