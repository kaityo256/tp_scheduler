#include "ising_mc.hpp"
#include <cmath>
#include <iostream>
#include <mpi.h>
#include <vector>

std::vector<double> temperatures = {1.80, 1.85, 1.90, 1.95, 2.00, 2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75};

void domc_all(void) {
  int size = 32;
  int rank, procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  int num_samples = procs;
  Params p;
  p.thermalization_loop = 1000;
  p.observation_loop = 1000;
  p.size = size;
  for (size_t i = 0; i < temperatures.size(); i++) {
    double t = temperatures[i];
    p.temperature = t;
    p.seed = i * num_samples + rank;
    std::vector<double> r = domc(p);
    std::vector<double> rbuf(r.size() * procs);
    MPI_Allgather(r.data(), r.size(), MPI_DOUBLE, rbuf.data(), r.size(), MPI_DOUBLE, MPI_COMM_WORLD);
    vvd data(procs);
    for (int j = 0; j < procs; j++) {
      data[j].resize(r.size());
      for (size_t k = 0; k < r.size(); k++) {
        data[j][k] = rbuf[j * r.size() + k];
      }
    }
    if (rank == 0) {
      calc_stdev(t, data);
    }
  }
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  domc_all();
  MPI_Finalize();
}
