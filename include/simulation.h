#ifndef EXTENSIONS_SIMULATION_H
#define EXTENSIONS_SIMULATION_H

#include <vector>
using std::vector;

namespace py_ext {
    double simulate(vector<vector<int>> incards, int nsim);
}

#endif // EXTENSIONS_SIMULATION_H