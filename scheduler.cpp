#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Worker {
    string id;
    string name;
    string contractType;
    int regularHoursPerDay;
    int maxOvertimeHoursPerDay;
    double efficiencyRate;
    int hourlyCost;
    bool heavyLifting;
    bool packing;
    bool qualityInspection;
    bool labeling;
};

struct Order {
    string id;
    string name;
    string complexity;
    int requiredWorkers;
    int deadlineDaysLeft;
    int priority;
    int totalItemQuantity;
    double estimatedHours;
    bool heavyLifting;
    bool packing;
    bool qualityInspection;
    bool labeling;
    string testCaseLabel;
};

struct Assignment {
    Order order;
    vector<int> team;
    int day;
    int startHour;
    int endHour;
    int duration;
};

struct Candidate {
    vector<int> team;
    int day;
    int startHour;
    int duration;
    int overtimeAdded;
};

vector<Worker> workers;
vector<Order> orders;
vector<Order> sortedOrders;
vector<Assignment> scheduleResult;
vector<vector<bool>> workerBusy;
vector<vector<int>> workerDailyHours;
int totalDays = 0;

vector<string> splitCsvLine(const string& line) {
    vector<string> cells;
    string cell;
    stringstream ss(line);

    while (getline(ss, cell, ',')) {
        cells.push_back(cell);
    }

    return cells;
}

bool toBool(const string& value) {
    return value == "1";
}

vector<Worker> readWorkers(const string& path) {
    ifstream file(path);
    vector<Worker> result;
    string line;

    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> cells = splitCsvLine(line);
        if (cells.size() < 11) {
            continue;
        }

        Worker worker;
        worker.id = cells[0];
        worker.name = cells[1];
        worker.contractType = cells[2];
        worker.regularHoursPerDay = stoi(cells[3]);
        worker.maxOvertimeHoursPerDay = stoi(cells[4]);
        worker.efficiencyRate = stod(cells[5]);
        worker.hourlyCost = stoi(cells[6]);
        worker.heavyLifting = toBool(cells[7]);
        worker.packing = toBool(cells[8]);
        worker.qualityInspection = toBool(cells[9]);
        worker.labeling = toBool(cells[10]);
        result.push_back(worker);
    }

    return result;
}

vector<Order> readOrders(const string& path) {
    ifstream file(path);
    vector<Order> result;
    string line;

    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> cells = splitCsvLine(line);
        if (cells.size() < 13) {
            continue;
        }

        Order order;
        order.id = cells[0];
        order.name = cells[1];
        order.complexity = cells[2];
        order.requiredWorkers = stoi(cells[3]);
        order.deadlineDaysLeft = stoi(cells[4]);
        order.priority = stoi(cells[5]);
        order.totalItemQuantity = stoi(cells[6]);
        order.estimatedHours = stod(cells[7]);
        order.heavyLifting = toBool(cells[8]);
        order.packing = toBool(cells[9]);
        order.qualityInspection = toBool(cells[10]);
        order.labeling = toBool(cells[11]);
        order.testCaseLabel = cells[12];
        result.push_back(order);
    }

    return result;
}

bool teamCoversSkills(const Order& order, const vector<int>& team) {
    bool heavy = false;
    bool packing = false;
    bool inspection = false;
    bool labeling = false;

    for (int workerIndex : team) {
        const Worker& worker = workers[workerIndex];
        heavy = heavy || worker.heavyLifting;
        packing = packing || worker.packing;
        inspection = inspection || worker.qualityInspection;
        labeling = labeling || worker.labeling;
    }

    return (!order.heavyLifting || heavy) &&
           (!order.packing || packing) &&
           (!order.qualityInspection || inspection) &&
           (!order.labeling || labeling);
}

double teamEfficiency(const vector<int>& team) {
    double total = 0.0;
    for (int workerIndex : team) {
        total += workers[workerIndex].efficiencyRate;
    }
    return total;
}

int teamCost(const vector<int>& team) {
    int total = 0;
    for (int workerIndex : team) {
        total += workers[workerIndex].hourlyCost;
    }
    return total;
}

int overtimeAddedByPlacement(const vector<int>& team, int day, int duration) {
    int overtimeAdded = 0;

    for (int workerIndex : team) {
        const Worker& worker = workers[workerIndex];
        int before = max(0, workerDailyHours[workerIndex][day] - worker.regularHoursPerDay);
        int after = max(0, workerDailyHours[workerIndex][day] + duration - worker.regularHoursPerDay);
        overtimeAdded += after - before;
    }

    return overtimeAdded;
}

void buildTeams(
    int startWorker,
    int remaining,
    vector<int>& current,
    vector<vector<int>>& teams
) {
    if (remaining == 0) {
        teams.push_back(current);
        return;
    }

    for (int i = startWorker; i <= static_cast<int>(workers.size()) - remaining; ++i) {
        current.push_back(i);
        buildTeams(i + 1, remaining - 1, current, teams);
        current.pop_back();
    }
}

vector<vector<int>> feasibleTeamsFor(const Order& order) {
    vector<vector<int>> teams;
    vector<int> current;
    buildTeams(0, order.requiredWorkers, current, teams);

    teams.erase(
        remove_if(
            teams.begin(),
            teams.end(),
            [&](const vector<int>& team) {
                return !teamCoversSkills(order, team);
            }
        ),
        teams.end()
    );

    sort(
        teams.begin(),
        teams.end(),
        [](const vector<int>& a, const vector<int>& b) {
            double efficiencyA = teamEfficiency(a);
            double efficiencyB = teamEfficiency(b);
            if (abs(efficiencyA - efficiencyB) > 0.0001) {
                return efficiencyA > efficiencyB;
            }
            return teamCost(a) < teamCost(b);
        }
    );

    return teams;
}

bool canPlace(const vector<int>& team, int day, int startHour, int duration) {
    int startSlot = day * 24 + startHour;
    int endSlot = startSlot + duration;

    for (int workerIndex : team) {
        const Worker& worker = workers[workerIndex];
        int maxDailyHours = worker.regularHoursPerDay + worker.maxOvertimeHoursPerDay;

        if (workerDailyHours[workerIndex][day] + duration > maxDailyHours) {
            return false;
        }

        for (int slot = startSlot; slot < endSlot; ++slot) {
            if (workerBusy[workerIndex][slot]) {
                return false;
            }
        }
    }

    return true;
}

void placeTeam(const vector<int>& team, int day, int startHour, int duration, bool busy) {
    int startSlot = day * 24 + startHour;
    int endSlot = startSlot + duration;
    int hourChange = busy ? duration : -duration;

    for (int workerIndex : team) {
        workerDailyHours[workerIndex][day] += hourChange;
        for (int slot = startSlot; slot < endSlot; ++slot) {
            workerBusy[workerIndex][slot] = busy;
        }
    }
}

bool scheduleFrom(int orderIndex) {
    if (orderIndex == static_cast<int>(sortedOrders.size())) {
        return true;
    }

    const Order& order = sortedOrders[orderIndex];
    vector<vector<int>> teams = feasibleTeamsFor(order);
    vector<Candidate> candidates;

    for (const vector<int>& team : teams) {
        int duration = static_cast<int>(ceil(order.estimatedHours / teamEfficiency(team)));
        if (duration <= 0 || duration > 24) {
            continue;
        }

        for (int day = 0; day < order.deadlineDaysLeft; ++day) {
            for (int startHour = 0; startHour + duration <= 24; ++startHour) {
                int endSlot = day * 24 + startHour + duration;
                int deadlineSlot = order.deadlineDaysLeft * 24;

                if (endSlot > deadlineSlot) {
                    continue;
                }

                if (canPlace(team, day, startHour, duration)) {
                    candidates.push_back({
                        team,
                        day,
                        startHour,
                        duration,
                        overtimeAddedByPlacement(team, day, duration)
                    });
                }
            }
        }
    }

    sort(
        candidates.begin(),
        candidates.end(),
        [](const Candidate& a, const Candidate& b) {
            int finishA = a.day * 24 + a.startHour + a.duration;
            int finishB = b.day * 24 + b.startHour + b.duration;
            if (finishA != finishB) {
                return finishA < finishB;
            }
            if (a.overtimeAdded != b.overtimeAdded) {
                return a.overtimeAdded < b.overtimeAdded;
            }
            int costA = teamCost(a.team);
            int costB = teamCost(b.team);
            if (costA != costB) {
                return costA < costB;
            }
            return teamEfficiency(a.team) > teamEfficiency(b.team);
        }
    );

    for (const Candidate& candidate : candidates) {
        placeTeam(candidate.team, candidate.day, candidate.startHour, candidate.duration, true);
        scheduleResult.push_back({
            order,
            candidate.team,
            candidate.day,
            candidate.startHour,
            candidate.startHour + candidate.duration,
            candidate.duration
        });

        if (scheduleFrom(orderIndex + 1)) {
            return true;
        }

        scheduleResult.pop_back();
        placeTeam(candidate.team, candidate.day, candidate.startHour, candidate.duration, false);
    }

    return false;
}

void printSchedule() {
    sort(
        scheduleResult.begin(),
        scheduleResult.end(),
        [](const Assignment& a, const Assignment& b) {
            if (a.day != b.day) {
                return a.day < b.day;
            }
            if (a.startHour != b.startHour) {
                return a.startHour < b.startHour;
            }
            return a.order.id < b.order.id;
        }
    );

    cout << "valid schedule found\n\n";

    for (const Assignment& assignment : scheduleResult) {
        cout << "day " << assignment.day + 1
             << ", " << assignment.startHour << ":00-" << assignment.endHour << ":00"
             << " | " << assignment.order.id
             << " | duration " << assignment.duration << "h"
             << " | team ";

        for (size_t i = 0; i < assignment.team.size(); ++i) {
            const Worker& worker = workers[assignment.team[i]];
            cout << worker.id << "(" << worker.name << ")";
            if (i + 1 < assignment.team.size()) {
                cout << ", ";
            }
        }

        cout << "\n";
    }

    cout << "\nworker daily hours\n";
    for (size_t workerIndex = 0; workerIndex < workers.size(); ++workerIndex) {
        bool printedWorker = false;
        for (int day = 0; day < totalDays; ++day) {
            int hours = workerDailyHours[workerIndex][day];
            if (hours == 0) {
                continue;
            }

            if (!printedWorker) {
                cout << workers[workerIndex].id << " " << workers[workerIndex].name << ": ";
                printedWorker = true;
            } else {
                cout << "; ";
            }

            cout << "day " << day + 1 << "=" << hours << "h";
        }

        if (printedWorker) {
            cout << "\n";
        }
    }
}

int main() {
    workers = readWorkers("workers.csv");
    orders = readOrders("orders.csv");

    if (workers.empty() || orders.empty()) {
        cout << "failed to read workers.csv or orders.csv\n";
        return 1;
    }

    sortedOrders = orders;
    sort(
        sortedOrders.begin(),
        sortedOrders.end(),
        [](const Order& a, const Order& b) {
            if (a.deadlineDaysLeft != b.deadlineDaysLeft) {
                return a.deadlineDaysLeft < b.deadlineDaysLeft;
            }
            if (a.priority != b.priority) {
                return a.priority > b.priority;
            }
            return a.estimatedHours > b.estimatedHours;
        }
    );

    for (const Order& order : orders) {
        totalDays = max(totalDays, order.deadlineDaysLeft);
    }

    workerBusy.assign(workers.size(), vector<bool>(totalDays * 24, false));
    workerDailyHours.assign(workers.size(), vector<int>(totalDays, 0));

    if (scheduleFrom(0)) {
        printSchedule();
        return 0;
    }

    cout << "no complete valid schedule found under the hourly backtracking model\n";
    return 2;
}
