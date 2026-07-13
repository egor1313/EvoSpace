#ifndef __NEURON_H__
#define __NEURON_H__
#include <vector>

class Neuron {
protected:
  int input_size_{0};
  std::vector<float> weights_;
  float output_{0.0};

public:
  Neuron(int input_size_, std::vector<float> &weights);
  void rand_weight(float minW, float maxW);
  float feed_forward(std::vector<float> &input);
  float getOutput() const;
};

#endif
