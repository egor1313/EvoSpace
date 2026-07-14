#include "neuron.h"
#include <chrono>
#include <random>

Neuron::Neuron(int input_size) : input_size_{input_size} {
  weights_.resize(input_size_, 0);
}
Neuron::Neuron(int input_size, std::vector<float> &weights)
    : input_size_{input_size} {
  weights_.resize(input_size_, 0);
  for (int i = 0; i < input_size_; ++i) {
    weights_[i] = weights[i];
  }
}
void Neuron::rand_weight(float minW, float maxW) {
  // BAD
  std::random_device rd;
  std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());

  std::uniform_real_distribution<float> fr(minW, maxW);
  for (int i = 0; i < input_size_; i++) {
    weights_[i] = fr(gen);
  }
}
float Neuron::feed_forward(const std::vector<float> &input) const {
  float output = 0.0;
  for (int i = 0; i < input_size_; i++) {
    output += input[i] * weights_[i];
  }
  output = 1.0 / (1.0 + exp(-output));
  return output;
}
