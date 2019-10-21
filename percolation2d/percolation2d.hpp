#pragma once
#include <vector>

struct Params {
  double parameter;
  int seed;
  int size;
  int observation_loop;
};

std::vector<double> percolation2d(Params &p);