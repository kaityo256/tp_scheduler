#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
int L; // System Size
int N; // Number of spins
std::vector<int> clusters;

void init(int size) {
  L = size;
  N = L * L;
  clusters.resize(N);
  for (int i = 0; i < N; i++) {
    clusters[i] = i;
  }
}

struct Params {
  double parameter;
  int seed;
  int size;
  int observation_loop;
};

int find(int i) {
  while (clusters[i] != i) {
    i = clusters[i];
  }
  return i;
}

void unite(int ix, int iy, int ix2, int iy2, double density, std::mt19937 &mt) {
  std::uniform_real_distribution<> ud(0.0, 1.0);
  if (ud(mt) > density) {
    return;
  }
  int i1 = ix + iy * L;
  int i2 = ix2 + iy2 * L;
  int c1 = find(i1);
  int c2 = find(i2);
  clusters[c1] = c2;
}

double crossing_probability(void) {
  for (int i = 0; i < L; i++) {
    int ci = find(i);
    for (int j = 0; j < L; j++) {
      int cj = find(j + (L - 1) * L);
      if (ci == cj) return 1.0;
    }
  }
  return 0.0;
}

double max_cluster_size(void) {
  std::vector<int> count(N, 0);
  for (int i = 0; i < N; i++) {
    int c = find(i);
    count[c]++;
  }
  int max = *std::max_element(count.begin(), count.end());
  return static_cast<double>(max) / static_cast<double>(N);
}

void clustering(double density, std::mt19937 &mt) {
  for (int iy = 0; iy < L - 1; iy++) {
    for (int ix = 0; ix < L - 1; ix++) {
      unite(ix, iy, ix + 1, iy, density, mt);
      unite(ix, iy, ix, iy + 1, density, mt);
    }
  }
}

void percolation2d(Params &p) {
  std::mt19937 mt(p.seed);
  const double density = p.parameter;
  double m = 0.0;  // max cluster size
  double cp = 0.0; // Crossing Probability
  for (int i = 0; i < p.observation_loop; i++) {
    init(p.size);
    clustering(density, mt);
    m += max_cluster_size();
    cp += crossing_probability();
  }
  m /= static_cast<double>(p.observation_loop);
  cp /= static_cast<double>(p.observation_loop);
  std::cout << density;
  std::cout << " " << m;
  std::cout << " " << cp << std::endl;
}

int main() {
  const double ds = 0.1;
  const double de = 0.9;
  const int ND = 20;
  for (int i = 0; i < ND; i++) {
    Params p;
    double density = ds + (de - ds) * i / static_cast<double>(ND);
    p.parameter = density;
    p.size = 32;
    p.seed = 1;
    p.observation_loop = 100;
    percolation2d(p);
  }
}