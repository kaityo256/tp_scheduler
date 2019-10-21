#pragma once
#include "parameters.hpp"
#include <cmath>
#include <iostream>
#include <vector>

struct Params {
  int size;
  double temperature;
  int seed;
  int thermalization_loop;
  int observation_loop;
};

std::vector<double> domc(const Params &p);
