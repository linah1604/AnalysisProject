#include <iostream>
#include <vector>
using namespace std;


struct worker {
    int worker_id;
    string worker_name;
    string contract_type;
    int regular_hours_per_day;
    int max_ovwertime_hours_per_day;
    int hourly_rate;
    double effiency_rate;
    bool skill_heavy_lifting;
    bool skill_packing;
    bool skill_quality_inspection;
    bool skill_labeling;
};

struct orders {
    int order_id;
    string order_name;
    string complexity;
    int deadline_days_left;
    int priority;
    int total_item_quantity;
    double estimated_total_hours;
    bool required_heavy_lifting;
    bool required_packing;
    bool required_quality_inspection;
    bool required_labeling;
};


struct orderItems{
    int order_id;
    string item_name;
    int item_quantity;
    double processing_hour_per_item;
    bool required_heavy_lifting;
    bool required_packing;
    bool required_quality_inspection;
    bool required_labeling;
};

// added after realizing we need to track the amount of workers and orders for the output 2d vector
vector<worker> workers;
vector<orders> orderList;
// added after realizing that we need to track the days left for each order, which will be updated as we assign hours to workers
vector<int> days;