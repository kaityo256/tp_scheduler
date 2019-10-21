#include "ising_mc.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int L; // System Size
int N; // Number of spins
double e_table[5];
std::vector<int> spin;
std::vector<std::array<int, 4>> neighbor;

void single_flip(std::mt19937 &mt) {
  std::uniform_real_distribution<double> ud(0.0, 1.0);
  for (int i = 0; i < N; i++) {
    int ns = 0;
    ns += spin[neighbor[i][0]];
    ns += spin[neighbor[i][1]];
    ns += spin[neighbor[i][2]];
    ns += spin[neighbor[i][3]];
    ns *= spin[i];
    if (e_table[ns / 2 + 2] > ud(mt)) {
      spin[i] = -spin[i];
    }
  }
}

double magnetization_square(void) {
  double m = std::accumulate(spin.begin(), spin.end(), 0.0);
  m /= static_cast<double>(N);
  return m * m;
}

void make_table(double beta) {
  for (int i = 0; i < 5; i++) {
    int ns = (i - 2) * 4;
    e_table[i] = exp(-ns * beta);
  }
}

int pos2index(int ix, int iy) {
  ix = (ix + L) % L;
  iy = (iy + L) % L;
  return ix + iy * L;
}

void init_neighbors() {
  for (int iy = 0; iy < L; iy++) {
    for (int ix = 0; ix < L; ix++) {
      int i = pos2index(ix, iy);
      neighbor[i][0] = pos2index(ix + 1, iy);
      neighbor[i][1] = pos2index(ix - 1, iy);
      neighbor[i][2] = pos2index(ix, iy + 1);
      neighbor[i][3] = pos2index(ix, iy - 1);
    }
  }
}

void init(int size, double beta) {
  L = size;
  N = L * L;
  spin.resize(N);
  std::fill(spin.begin(), spin.end(), 1);
  neighbor.resize(N);
  init_neighbors();
  make_table(beta);
}

std::vector<double> ising2d(const Params &p) {
  std::mt19937 mt(p.seed);
  double beta = 1.0 / p.parameter;
  init(p.size, beta);
  for (int j = 0; j < p.thermalization_loop; j++) {
    single_flip(mt);
  }
  double sm2 = 0.0, sm4 = 0.0;
  for (int k = 0; k < p.observation_loop; k++) {
    single_flip(mt);
    double m2 = magnetization_square();
    sm2 += m2;
    sm4 += m2 * m2;
  }
  sm2 /= static_cast<double>(p.observation_loop);
  sm4 /= static_cast<double>(p.observation_loop);
  double sU = sm4 / (sm2 * sm2);
  std::vector<double> r;
  r.push_back(sm2);
  r.push_back(sU);
  return r;
}
