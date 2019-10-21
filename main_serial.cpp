#include "ising_mc.hpp"
#include <cmath>
#include <iostream>
#include <vector>

template <class FUNC>
void domc_all(std::vector<Params> &pv, FUNC &run) {
  for (size_t i = 0; i < pv.size(); i++) {
    std::vector<std::vector<double>> data;
    for (int j = 0; j < num_samples; j++) {
      pv[i].seed = j;
      std::vector<double> r = run(pv[i]);
      data.push_back(r);
    }
    calc_stdev(pv[i].temperature, data);
  }
}

int main(void) {
  // Tempeartures to be simulated
  std::vector<double> temperatures = {1.80, 1.85, 1.90, 1.95, 2.00, 2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75};
  std::vector<Params> pv;
  for (size_t i = 0; i < temperatures.size(); i++) {
    Params p;
    p.thermalization_loop = thermalization_loop;
    p.observation_loop = observation_loop;
    p.size = size;
    p.temperature = temperatures[i];
    pv.push_back(p);
  }
  domc_all(pv, domc);
}
