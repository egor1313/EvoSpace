#include "entity.h"
#include "iostream"
#include "planet.h"

void simulation_step(std::vector<Entity> &es, std::vector<Planet> &ps);
int main() {
  std::vector<Entity> es;
  std::vector<Planet> ps;
  while (true) {
    simulation_step(es, ps);
  }
  return 0;
}
