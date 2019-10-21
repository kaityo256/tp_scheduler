# Trivial Parallelization Scheduler

## Summary

This is a sample code for trivial parallelization scheduler. Trivial parallelization, aka, embarrassingly parallelization is a basic technique to utilize the computational power of parallel machine.

There are two kinds of tirivial parallelization. One is the parallelization on the parameter space. Suppose we study the temperature dependence of something. If the simulations at the different temperatures are independent each other, they can be performed simultaneously. The other is the parallelizationon the sampling space. We usually perform the simulation with same parameters but with different seed in order to estimate error bars. Of cource the runs with different seeds can be performed simultaneously.

This scheduler adopts two kinds of trivial parallelization simultaneously. Namely, we divide the processes into several groups and assign different tasks to each group.

Suppose we have 20 processes. We divide them into five groups
 by using `MPI_Comm_split`.

```txt
0 1 2 3 | 4 5 6 7 | 8 9 10 11 | 12 13 14 15 | 16 17 18 19
```

A different group has different color. The four processes in a group are used for sampling, i.e., they perform simulations with same parameters but with different seeds. Then we can estimate the error bars.

Different colors are used for parallelization on the parameter space. For example, we want to perform simulations at 20 temperature points. Then the temperatures are divided into five groups. Each temperature group is assigned to each process group.

This scheduler allows us to perform such trivial parallelization for parameters and samplings simultaneously.

## Usage

Here is the sample for a serial run. This is a Monte Carlo simulation of the ferromagnetic Ising model on the square lattice. The spins are updated by the Metropolis method.

```cpp
#include "../tps.hpp"
#include "ising_mc.hpp"
#include <cmath>
#include <iostream>
#include <vector>

const int num_samples = 4;             // Number of Samplings at each temperatures
const int size = 32;                   // System Size
const int thermalization_loop = 10000; // Number of Loops for Thermalization
const int observation_loop = 1000;     // Number of Loops for Observation

int main(void) {
  // Tempeartures to be simulated
  std::vector<double> temperatures = {1.80, 1.85, 1.90, 1.95, 2.00, 2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75};
  std::vector<Params> pv;
  for (size_t i = 0; i < temperatures.size(); i++) {
    Params p;
    p.thermalization_loop = thermalization_loop;
    p.observation_loop = observation_loop;
    p.size = size;
    p.parameter = temperatures[i];
    pv.push_back(p);
  }
  tps::run(pv, ising2d, num_samples);
}
```

The function `ising2d` is the simulator. It has the folowing interface.

```cpp
std::vector<double> ising2d(const Params &p);
```

It receives parameters via `Param` structure and returns observables via `std::vector<double>`. The structure `Param` must contain two fields, `parameter` and `seed`. Other fields are optional.

The scheduler `tps::run` executes the simulator  `num_samples` times for each parameter point.