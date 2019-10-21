#pragma once
#include <cmath>
#include <iostream>
#include <vector>

namespace tps {

__attribute__((weak)) void calc_stdev(const double temperature, const std::vector<std::vector<double>> &data) {
  std::vector<double> vr, vr2;
  const int num_samples = data.size();
  vr.resize(data[0].size());
  vr2.resize(data[0].size());
  for (size_t j = 0; j < data.size(); j++) {
    for (size_t k = 0; k < data[0].size(); k++) {
      vr[k] += data[j][k] / static_cast<double>(num_samples);
      vr2[k] += (data[j][k] * data[j][k]) / static_cast<double>(num_samples);
    }
  }
  std::cout << temperature;
  for (size_t j = 0; j < vr.size(); j++) {
    std::cout << " " << vr[j];
    const double var = (vr2[j] - vr[j] * vr[j]) / static_cast<double>(num_samples - 1);
    std::cout << " +- " << std::sqrt(var);
  }
  std::cout << std::endl;
}

template <class FUNC, class PARAMS>
void run(std::vector<PARAMS> &pv, FUNC &run_task, int num_samples) {
  for (size_t i = 0; i < pv.size(); i++) {
    std::vector<std::vector<double>> data;
    for (int j = 0; j < num_samples; j++) {
      pv[i].seed = j;
      std::vector<double> r = run_task(pv[i]);
      data.push_back(r);
    }
    calc_stdev(pv[i].temperature, data);
  }
}

} // namespace tps