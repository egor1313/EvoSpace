#ifndef __LAYER_H__
#define __LAYER_H__
#include "neuron.h"
#include <vector>

class Layer {
protected:
  int layer_size_{0};
  int input_size_{0};
  std::vector<Neuron> layer_;

public:
  Layer() = default;
  Layer(int input_size, int neuron_count);
  std::vector<float> feed_forward(const std::vector<float> &input) const;
  int get_input_size() const;
};

#endif
