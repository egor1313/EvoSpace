#include "network.h"
#include <iostream>

Network::Network(const std::vector<int> &topology)
    : network_size_(topology.size()), input_size_{topology[0]},
      output_size_{topology[network_size_ - 1]} {
  for (int i = 1; i < network_size_; ++i) {
    layers_[i] = Layer(topology[i - 1], topology[i]);
  }
}
std::vector<float>
Network::feed_forward(const std::vector<float> &input) const {
  if (layers_.empty())
    std::cout << "NN EMPTY NN" << std::endl;
  if (input.size() != layers_[0].get_input_size())
    std::cout << "NN WRONG INPUT" << std::endl;
  std::vector<float> cur = layers_[0].feed_forward(input);
  for (int i = 1; i < network_size_; ++i) {
    cur = layers_[i].feed_forward(input);
  }
  return cur;
}
