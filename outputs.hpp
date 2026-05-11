// a 2d vector with one dimension being the worker ids and the other dimension being the order ids, and the value being the hours assigned to that worker for that order
#include <vector>
#include "inputs.hpp"
using namespace std;

// rows = workers
// columns = orders
// value = assigned hours

vector<vector<double>> workerOrderHours(
    workers.size(),
    vector<double>(orderList.size(), 0.0)
);