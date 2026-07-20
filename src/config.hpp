#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace Config {
inline constexpr int MaxLayerNeurons = 15;
inline constexpr int MaxLayers = 7;
class Balance {
public:
  static Balance &Get() {
    static Balance instance;
    return instance;
  }

  float kStandartMutationForce = 0.5f;
  double kStandartPowerOfEngine = 0.5f;
  float kStandartEyePower = 0.5f;
  double kStandartAngle = 0.0f;
  float kStandartEnergy = 100.0f;
  int kStandartLifeTime = 100;
  float kStandartBirthNeedEnergy = 10.0f;

private:
  Balance() { LoadFromFile("balance.json"); }
  void LoadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Warning: balance.json not found. \n";
    }

    try {
      nlohmann::json j;
      file >> j;
      if (j.contains("kStandartMutationForce"))
        j.at("kStandartMutationForce").get_to(kStandartMutationForce);
      if (j.contains("kStandartPowerOfEngine"))
        j.at("kStandartPowerOfEngine").get_to(kStandartPowerOfEngine);
      if (j.contains("kStandartEyePower"))
        j.at("kStandartEyePower").get_to(kStandartEyePower);
      if (j.contains("kStandartEnergy"))
        j.at("kStandartEnergy").get_to(kStandartEnergy);
      if (j.contains("kStandartAngle"))
        j.at("kStandartAngle").get_to(kStandartAngle);
      if (j.contains("kStandartLifeTime"))
        j.at("kStandartLifeTime").get_to(kStandartLifeTime);
      if (j.contains("kStandartBirthNeedEnergy"))
        j.at("kStandartBirthNeedEnergy").get_to(kStandartBirthNeedEnergy);
    } catch (const std::exception &e) {
      std::cerr << "Error parsing balance.json: " << e.what() << "\n";
    }
  }
};

} // namespace Config

#endif
