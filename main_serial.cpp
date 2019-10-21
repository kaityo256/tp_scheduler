#include "ising_mc.hpp"
#include <cmath>
#include <iostream>
#include <vector>

const int num_samples = 4;

void domc_all(std::vector<double> temperatures) {
  int size = 32;
  Params p;
  p.thermalization_loop = 1000;
  p.observation_loop = 1000;
  p.size = size;
  for (size_t i = 0; i < temperatures.size(); i++) {
    double t = temperatures[i];
    p.temperature = t;
    vvd data;
    for (int j = 0; j < num_samples; j++) {
      p.seed = j;
      std::vector<double> r = domc(p);
      data.push_back(r);
    }
    calc_stdev(t, data);
  }
}

int main(void) {
  std::vector<double> temperatures = {1.80, 1.85, 1.90, 1.95, 2.00, 2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75};
  domc_all(temperatures);
}
