#pragma once
#include <cmath>
#include <iostream>
#include <vector>

const int size = 32;       // System Size
const int num_samples = 4; // Number of Samplings at each temperatures
const int thermalization_loop = 10000;
const int observation_loop = 1000;

struct Params {
  int size;
  double temperature;
  int seed;
  int thermalization_loop;
  int observation_loop;
};

std::vector<double> domc(const Params &p);

typedef std::vector<std::vector<double>> vvd;

__attribute__ ((weak))  void calc_stdev(const double temperature, const vvd &data) {
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
