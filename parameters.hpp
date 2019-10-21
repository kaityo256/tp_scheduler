const int size = 32;       // System Size
const int num_samples = 4; // Number of Samplings at each temperatures
const int thermalization_loop = 10000; // Number of Loops for Thermalization
const int observation_loop = 1000;     // Number of Loops for Observation

// Temperatures to be simulated
const std::vector<double> temperatures = {1.80, 1.85, 1.90, 1.95, 2.00, 2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50, 2.55, 2.60, 2.65, 2.70, 2.75};
