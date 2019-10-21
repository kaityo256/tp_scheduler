#include "ising_mc.hpp"
#include <cmath>
#include <iostream>
#include <mpi.h>
#include <vector>

void domc_all(std::vector<double> &my_t, MPI_Comm &my_comm) {
  int size = 32;
  int rank, procs;
  int grank, gprocs;
  MPI_Comm_rank(my_comm, &rank);
  MPI_Comm_size(my_comm, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &grank);
  MPI_Comm_size(MPI_COMM_WORLD, &gprocs);
  int num_colors = gprocs/procs;
  int csize = gprocs / num_colors;
  int my_color = grank / csize;
  int num_samples = procs;
  Params p;
  p.thermalization_loop = 1000;
  p.observation_loop = 1000;
  p.size = size;
  for (size_t i = 0; i < my_t.size(); i++) {
    double t = my_t[i];
    p.temperature = t;
    p.seed = rank;
    //printf("%02d : %f\n",p.seed, t);
    std::vector<double> r = domc(p);
    std::vector<double> rbuf(r.size() * procs);
    MPI_Allgather(r.data(), r.size(), MPI_DOUBLE, rbuf.data(), r.size(), MPI_DOUBLE, my_comm);
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

void dummy(std::vector<double> &my_t, MPI_Comm &my_comm) {
  int rank, procs;
  int grank, gprocs;
  MPI_Comm_rank(my_comm, &rank);
  MPI_Comm_size(my_comm, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &grank);
  MPI_Comm_size(MPI_COMM_WORLD, &gprocs);
  int num_colors = gprocs/procs;
  int my_color = grank / procs;
  printf("%d %d\n",grank, my_color);

}

int main(int argc, char **argv) {
std::vector<double> temperatures = {1.80, 1.85, 1.90, 1.95, 2.00, 2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75};
  MPI_Init(&argc, &argv);
  int rank, procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm my_comm = MPI_COMM_WORLD;
  int csize = 4;
  int color = rank / csize;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &my_comm);
  std::vector<double> my_t(temperatures.begin() + color*csize, temperatures.begin() + (color+1)*csize);
  //dummy(my_t, my_comm);
  domc_all(my_t, my_comm);
  MPI_Finalize();
}
