#pragma once
#include <cmath>
#include <iostream>
#include <vector>

struct Params {
  int size;
  double parameter;
  int seed;
  int thermalization_loop;
  int observation_loop;
};

std::vector<double> ising2d(const Params &p);
