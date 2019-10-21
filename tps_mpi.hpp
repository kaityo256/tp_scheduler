#pragma once

#include "tps.hpp"
#include <mpi.h>

namespace tps {
template <class FUNC, class PARAMS>
void run_mpi_comm(std::vector<PARAMS> &pv, FUNC &run, MPI_Comm &my_comm) {
  int rank, procs;
  int grank, gprocs;
  MPI_Comm_rank(my_comm, &rank);
  MPI_Comm_size(my_comm, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &grank);
  MPI_Comm_size(MPI_COMM_WORLD, &gprocs);
  for (size_t i = 0; i < pv.size(); i++) {
    double t = pv[i].parameter;
    pv[i].seed = rank;
    std::vector<double> r = run(pv[i]);
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
      tps::calc_stdev(t, data);
    }
  }
}

template <class FUNC, class PARAMS>
void run_mpi(std::vector<PARAMS> &pv, FUNC &run_task, int num_samples) {
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
  int num_data = pv.size();
  int block = num_data / num_colors;
  int rest = num_data % num_colors;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &my_comm);
  int s = color * block + std::min(color, rest);
  int e = s + block;
  if (color < rest) {
    e += 1;
  }
  std::vector<PARAMS> my_param(pv.begin() + s, pv.begin() + e);
  run_mpi_comm(my_param, run_task, my_comm);
}
} // namespace tps