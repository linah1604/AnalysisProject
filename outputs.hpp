/* a 2d vector with one dimension being the worker ids and the other dimension being the order ids, and the value being the hours assigned to that worker for that order
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

*/

// realized we need a 3d vector instead of 2d to track the hours assigned to each worker for each order on each day, 
// since the hours assigned to a worker for an order can change from day to day as we update the days left for each order and reassign hours to workers accordingly


#include <iostream>
#include <vector>
#include "inputs.hpp"
using namespace std;

// [days] [workers] [orders]
// value = assigned hours

vector<vector<vector<double>>> dayWorkerOrderHours(
    days.size(),
    vector<vector<double>>(
        workers.size(),
        vector<double>(orderList.size(), 0.0)
    )
);