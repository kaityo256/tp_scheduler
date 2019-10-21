#include "../tps.hpp"
#include "percolation2d.hpp"
#include <vector>

const int num_samples = 4;

int main() {
  std::vector<double> densities = {0.40, 0.59, 0.41, 0.58, 0.42, 0.57, 0.43, 0.56, 0.44, 0.55, 0.45, 0.54, 0.46, 0.53, 0.47, 0.52, 0.48, 0.51, 0.49, 0.50};
  std::vector<Params> pv;
  for (size_t i = 0; i < densities.size(); i++) {
    Params p;
    p.parameter = densities[i];
    p.size = 64;
    p.seed = 1;
    p.observation_loop = 100;
    pv.push_back(p);
  }
  tps::run(pv, percolation2d, num_samples);
}