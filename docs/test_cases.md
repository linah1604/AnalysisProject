# Test Cases

Test cases are embedded in `data/orders.csv` via the `test_case_label` column. Each label groups one or more orders that exercise a specific scheduling scenario.

---

## TC01_SIMPLE — Single-worker low-complexity order

**Orders:** O01, O10

**Setup:** One worker is sufficient. No heavy lifting required. All four standard skills needed but any single skilled worker can cover them.

**Expected behavior:** Scheduler assigns one worker per order. The assignment fits within a single day well inside the deadline.

**Verified by:** O10 (`Simple living room accessories`, LOW, 1 worker, deadline 2 days) assigned to W07(Karim) in 3 hours on day 1 in the sample run.

---

## TC02_MIXED — Medium complexity with heavy lifting

**Orders:** O02, O05, O08

**Setup:** Requires 2 workers and heavy lifting skill. Tests that the team selection filter correctly rejects workers without the required skill.

**Expected behavior:** Both team members must cover all required skills collectively. The scheduler pairs a heavy-lifting worker with a multi-skilled partner.

**Verified by:** O05 (`Bedroom essentials order`, MEDIUM, 2 workers, heavy lifting) assigned to W03(Youssef) + W09(Hassan) in 3 hours on day 1.

---

## TC03_TEAM — High complexity requiring a 2–3 worker team

**Orders:** O03, O04, O07

**Setup:** `required_workers` is 2 or 3. All team members must work the exact same time interval. Tests simultaneous placement and combined efficiency scaling.

**Expected behavior:** All team workers share an identical start and end slot. Duration is reduced by combined efficiency.

**Verified by:** O03 (`Living room heavy furniture order`, HIGH, 2 workers) assigned to W01(Ahmed) + W02(Mona), 4 hours, day 1.

---

## TC04_LIMITED_SKILLS — Order blocked by skill scarcity

**Orders:** O08

**Setup:** Requires heavy lifting plus full skill coverage with 2 workers. Tests that infeasible teams are pruned before the search.

**Expected behavior:** Only worker combinations where at least one member has heavy lifting are considered.

---

## TC05_TIGHT_DEADLINE — Order due in 1 day

**Orders:** O06, O11

**Setup:** Deadline is 1 day. Tests that earliest-deadline-first ordering places this order before all others and that it fits within the one available day.

**Expected behavior:** O06 is scheduled on day 1 before orders with looser deadlines.

**Verified by:** O06 (`Urgent small delivery order`, deadline 1 day) assigned to W01(Ahmed), 0:00–2:00, day 1 in the sample run.

---

## TC06_HIGH_WORKLOAD — Parallel high-complexity orders

**Orders:** O09, O12

**Setup:** Both orders are HIGH complexity with 3 required workers and long processing times. Tests that the scheduler can pack multiple large orders without worker conflicts.

**Expected behavior:** Workers are partitioned across orders without overlap. Backtracking resolves any conflicts.

---

## Failure Case

Remove all workers who possess heavy lifting skill from `workers.csv` and re-run. The scheduler should print:

```
no complete valid schedule found under the hourly backtracking model
```

and exit with code 2.
