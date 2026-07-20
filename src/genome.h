#ifndef __GENOME_H__
#define __GENOME_H__

#include "config.hpp"
#include <random>
#include <vector>

// Параметры сущности
struct Phenotype {
  double power_of_engine;
  float eye_power;

  Phenotype(double p = Config::Balance::Get().kStandartPowerOfEngine,
            float eye = Config::Balance::Get().kStandartEyePower)
      : power_of_engine(p), eye_power(eye) {}
};

class Genome {
private:
  std::vector<int> topology_{};
  std::vector<float> weights_{};
  Phenotype params_{};

  int compute_weights_count(const std::vector<int> &topo) const;

public:
  Genome() = default;
  Genome(const std::vector<int> &topology,
         const Phenotype &params = Phenotype());
  Genome(const std::vector<int> &topology, const std::vector<float> &weights,
         const Phenotype &params);

  // Геттеры
  const std::vector<int> &get_topology() const { return topology_; }
  const std::vector<float> &get_weights() const { return weights_; }
  const Phenotype &get_params() const { return params_; }
  void set_params(const Phenotype &params) { params_ = params; }

  // Инициализация случайными значениями
  void randomize(float minW = -1.0f, float maxW = 1.0f, float minPower = 0.0f,
                 float maxPower = 10.0f, float minEye = 0.0f,
                 float maxEye = 10.0f);

  // Мутация (отдельные вероятности и амплитуды для весов и параметров)
  void mutate(float weight_rate = 0.1f, float weight_sigma = 0.5f,
              float param_rate = 0.1f, float param_sigma = 0.5f);

  // Кроссинговер (одноточечный для весов, усреднение для параметров)
  Genome crossover(const Genome &other) const;

  // Проверка совместимости топологий
  bool is_compatible(const Genome &other) const;
};

#endif
