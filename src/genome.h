#ifndef __GENOME_H__
#define __GENOME_H__

#include <random>
#include <vector>

// Параметры сущности
struct EntityParams {
  float energy;
  float power_of_engine;
  float eye_power;
  float age;

  EntityParams(float e = 0.0f, float p = 0.0f, float eye = 0.0f)
      : energy(e), power_of_engine(p), eye_power(eye), age(0) {}
};

class Genome {
private:
  std::vector<int> topology_;
  std::vector<float> weights_;
  EntityParams params_;

  int compute_weights_count(const std::vector<int> &topo) const;

public:
  Genome() = default;
  Genome(const std::vector<int> &topology,
         const EntityParams &params = EntityParams());
  Genome(const std::vector<int> &topology, const std::vector<float> &weights,
         const EntityParams &params);

  // Геттеры
  const std::vector<int> &get_topology() const { return topology_; }
  const std::vector<float> &get_weights() const { return weights_; }
  const EntityParams &get_params() const { return params_; }
  void set_params(const EntityParams &params) { params_ = params; }

  // Инициализация случайными значениями
  void randomize(float minW = -1.0f, float maxW = 1.0f, float minEnergy = 0.0f,
                 float maxEnergy = 100.0f, float minPower = 0.0f,
                 float maxPower = 10.0f, float minEye = 0.0f,
                 float maxEye = 10.0f);

  // Мутация (отдельные вероятности и амплитуды для весов и параметров)
  void mutate(float weight_rate = 0.1f, float weight_sigma = 0.5f,
              float param_rate = 0.1f, float param_sigma = 0.5f);

  // Кроссовер (одноточечный для весов, усреднение для параметров)
  Genome crossover(const Genome &other) const;

  // Проверка совместимости топологий
  bool is_compatible(const Genome &other) const;
};

#endif
