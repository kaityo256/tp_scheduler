#include "ising_mc.hpp"
#include "tps_mpi.hpp"
#include <cmath>
#include <iostream>
#include <mpi.h>
#include <vector>

const int num_samples = 4;             // Number of Samplings at each temperatures
const int size = 32;                   // System Size
const int thermalization_loop = 10000; // Number of Loops for Thermalization
const int observation_loop = 1000;     // Number of Loops for Observation

int main(int argc, char **argv) {
  // Tempeartures to be simulated
  std::vector<double> temperatures = {1.80, 1.85, 1.90, 1.95, 2.00, 2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75};
  std::vector<Params> pv;
  for (size_t i = 0; i < temperatures.size(); i++) {
    Params p;
    p.thermalization_loop = thermalization_loop;
    p.observation_loop = observation_loop;
    p.size = size;
    p.parameter = temperatures[i];
    pv.push_back(p);
  }
  MPI_Init(&argc, &argv);
  tps::run_mpi(pv, domc, num_samples);
  MPI_Finalize();
}
