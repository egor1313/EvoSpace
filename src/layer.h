#ifndef __LAYER_H__
#define __LAYER_H__
#include "neuron.h"
#include <vector>

class Layer {
protected:
  int layer_size_{0};
  int input_size_{0};
  int output_size_{0};
  std::vector<Neuron> layer_;

public:
  Layer(int input_size_, int output_size_);
  std::vector<float> feed_forward(std::vector<float> &input);
};

#endif
