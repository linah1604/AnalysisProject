# AnalysisProject

## High-Level Overview
This project reads worker and order data from CSV files, builds the in-memory data model, and searches for a valid work schedule that matches worker skills, daily hour limits, and order deadlines.

## Files
- `inputs.hpp` (`a1`): defines the worker, order, and order-item input structures plus shared containers.
- `outputs.hpp` (`a1`): defines the 3D assignment tracking structure used to store hours by day, worker, and order.
- `scheduler.cpp` (`a2`): reads CSV data, builds feasible teams, backtracks through candidate placements, and prints the final schedule.

## Complexity
- `inputs.hpp`: O(1) time and O(1) space for the declarations themselves.
- `outputs.hpp`: O(1) time and O(1) space for the declarations themselves.
- `scheduler.cpp`: worst-case exponential time because it enumerates worker teams and recursively backtracks over candidate schedule placements; space is polynomial, driven by the worker/day tracking tables and the recursion stack.
