#ifndef __NETWORK_H__
#define __NETWORK_H__
#include "layer.h"
#include <vector>

class Network {
protected:
  int network_size_{0};
  int input_size_{0};
  int output_size_{0};
  std::vector<Layer> layers_{};

public:
  Network() = default;
  Network(const std::vector<int> &topology);
  std::vector<float> feed_forward(const std::vector<float> &input) const;
};

#endif
