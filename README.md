# AnalysisProject — Warehouse Worker Scheduler

## Overview

A warehouse receives daily customer orders that must be packed and shipped within deadlines. This project builds a backtracking scheduler that assigns workers to orders, respecting skill requirements, team sizes, daily hour limits, and exact time-slot availability.

## Repository Structure

```
AnalysisProject/
├── scheduler.cpp               source — backtracking scheduler (A2)
├── inputs.hpp                  source — input struct definitions (A1)
├── outputs.hpp                 source — output struct definitions (A1)
├── data/
│   ├── workers.csv             worker roster with skills and hour limits
│   ├── orders.csv              orders with deadlines, skill requirements, and team sizes
│   └── order_items.csv         per-item breakdown of each order
└── docs/
    ├── algorithm_analysis.md   problem abstraction, pseudocode, correctness, and complexity
    ├── io_explanation.md       input/output format and field descriptions
    ├── test_cases.md           test cases and expected behavior
    ├── execution_log.txt       sample run output on a reduced dataset
    └── ai_contribution.md      breakdown of AI-generated vs. student-written content
```

## How to Build and Run

The scheduler reads `workers.csv` and `orders.csv` from the working directory.

```bash
g++ -std=c++17 scheduler.cpp -o scheduler
cp data/workers.csv data/orders.csv .
./scheduler
```

On success the schedule is printed to stdout. On failure the program prints a message and exits with code 2.

## Complexity Summary

| File | Time | Space |
|---|---|---|
| `scheduler.cpp` | O((K·T)^O) worst case | O(W·T + O) |

Where O = orders, W = workers, T = hourly time slots, K = feasible teams per order.
