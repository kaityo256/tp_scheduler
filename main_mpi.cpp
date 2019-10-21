#include "ising_mc.hpp"
#include <cmath>
#include <iostream>
#include <mpi.h>
#include <vector>

void domc_all(std::vector<Params> &pv, MPI_Comm &my_comm) {
  int rank, procs;
  int grank, gprocs;
  MPI_Comm_rank(my_comm, &rank);
  MPI_Comm_size(my_comm, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &grank);
  MPI_Comm_size(MPI_COMM_WORLD, &gprocs);
  for (size_t i = 0; i < pv.size(); i++) {
    double t = pv[i].temperature;
    pv[i].seed = rank;
    std::vector<double> r = domc(pv[i]);
    std::vector<double> rbuf(r.size() * procs);
    MPI_Allgather(r.data(), r.size(), MPI_DOUBLE, rbuf.data(), r.size(), MPI_DOUBLE, my_comm);
    std::vector<std::vector<double>> data(procs);
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
  MPI_Init(&argc, &argv);
  int rank, procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm my_comm = MPI_COMM_WORLD;
  if (procs % num_samples != 0 && rank == 0) {
    std::cerr << "Number of procs must be multiple of num_samples" << std::endl;
    MPI_Abort(MPI_COMM_WORLD, -1);
  }
  int csize = num_samples;
  int color = rank / csize;
  int num_colors = procs / csize;
  int num_data = temperatures.size();
  int block = num_data / num_colors;
  int rest = num_data % num_colors;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &my_comm);
  int s = color * block + std::min(color, rest);
  int e = s + block;
  if (color < rest) {
    e += 1;
  }
  std::vector<Params> my_t(pv.begin() + s, pv.begin() + e);
  domc_all(my_t, my_comm);
  MPI_Finalize();
}
