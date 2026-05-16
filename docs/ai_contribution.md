# AI vs. Student Contribution

This document identifies which parts of the project were AI-generated and which were written or significantly modified by the students.

---

## Student-Written

### Problem understanding and design decisions
- Identifying that the A1 greedy baseline fails to prevent intra-day worker overlap (described in `algorithm_analysis.md` under "baseline algorithm" and "human development notes").
- Decision to switch from daily-hour totals to exact 1-hour time slots to guarantee no double-booking.
- Decision to require all team members to share the exact same start and end slot for multi-worker orders.
- Keeping overtime allowed but bounded by per-worker limits from the dataset.

### A1 deliverables (`inputs.hpp`, `outputs.hpp`)
- The struct definitions and variable names in `inputs.hpp` were written by the students.
- The 3D vector layout in `outputs.hpp` (`dayWorkerOrderHours[day][worker][order]`) and the rationale for upgrading from a 2D to a 3D structure were worked out by the students (the comment history in the file shows the intermediate thinking).

### Dataset (`data/`)
- `workers.csv`, `orders.csv`, and `order_items.csv` were designed by the students to reflect realistic warehouse scenarios and to cover the specific test cases documented in `docs/test_cases.md`.

### Algorithm analysis (`docs/algorithm_analysis.md`)
- The problem abstraction, industrial considerations, baseline limitation analysis, and the time log reflect student thinking.
- The pseudocode structure and constraint list were drafted by the students.
- The correctness argument and complexity derivation were written by the students and then reviewed with AI assistance for phrasing.

---

## AI-Generated or AI-Assisted

### `scheduler.cpp` — primary implementation
The full implementation of `scheduler.cpp` was generated with AI assistance based on the pseudocode, constraints, and design decisions documented by the students. The students provided:
- The pseudocode and constraint list as the specification.
- Feedback on intermediate outputs (e.g. verifying that team orders use a shared slot, that overtime is tracked per-day not per-slot).

Specific parts the students reviewed and confirmed:
- `feasibleTeamsFor`: team enumeration and skill filtering logic.
- `canPlace` / `placeTeam`: slot-level availability check and hour tracking.
- `scheduleFrom`: backtracking structure and candidate sorting priority.
- `printSchedule`: output format verified against expected layout.

### Phrasing and formatting in docs
- Some prose in `algorithm_analysis.md` (correctness argument, complexity section) was refined with AI assistance for clarity and formatting.

---

## Summary

| Artifact | Primary author |
|---|---|
| `inputs.hpp` | Student |
| `outputs.hpp` | Student |
| `scheduler.cpp` | AI-generated, student-specified and reviewed |
| `data/*.csv` | Student |
| Problem framing and design rationale | Student |
| Pseudocode and constraints | Student |
| Algorithm analysis prose | Student + AI editing |
| `docs/` (this project) | AI-generated, student-reviewed |
