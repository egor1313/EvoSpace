#include "layer.h"

Layer::Layer(int input_size, int neuron_count)
    : input_size_{input_size}, layer_size_{neuron_count} {
  layer_.resize(layer_size_, Neuron(input_size_));
}
std::vector<float> Layer::feed_forward(const std::vector<float> &input) const {
  std::vector<float> a(layer_size_);
  for (int i = 0; i < layer_size_; i++) {
    a[i] = layer_[i].feed_forward(input);
  }
  return a;
}
int Layer::get_input_size() const { return input_size_; }
