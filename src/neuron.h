#ifndef __NEURON_H__
#define __NEURON_H__
#include <vector>

class Neuron {
protected:
  int input_size_{0};
  std::vector<float> weights_{};

public:
  Neuron() = default;
  Neuron(int input_size);
  Neuron(int input_size, std::vector<float> &weights);
  void rand_weight(float minW, float maxW);
  float feed_forward(const std::vector<float> &input) const;
};

#endif
