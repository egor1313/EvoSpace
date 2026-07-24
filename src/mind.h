#ifndef __MIND_H__
#define __MIND_H__
#include "genome.h"
#include "network.h"
#include <vector>

class Mind {
protected:
  Network net{};

public:
  Mind() = default;
  Mind(Genome gen);
  Mind(const std::vector<int> &top);
  Genome &get_mind_genome() const;
  void set_mind_genome(const Genome &gen);
};

#endif
