# Input / Output Explanation

## Inputs

### `data/workers.csv`

One row per worker.

| Column | Type | Description |
|---|---|---|
| `worker_id` | string | Unique identifier (e.g. `W01`) |
| `worker_name` | string | Display name |
| `contract_type` | string | `Regular` or `Part-time` |
| `regular_hours_per_day` | int | Normal shift length in hours |
| `max_overtime_hours_per_day` | int | Extra hours allowed beyond the regular shift |
| `efficiency_rate` | float | Multiplier applied to order processing time (1.0 = baseline) |
| `hourly_cost` | int | Cost per hour in currency units |
| `skill_heavy_item_lifting` | 0/1 | Can handle heavy items |
| `skill_packing` | 0/1 | Can pack items |
| `skill_quality_inspection` | 0/1 | Can perform quality checks |
| `skill_labeling_before_delivery` | 0/1 | Can label items |

### `data/orders.csv`

One row per order.

| Column | Type | Description |
|---|---|---|
| `order_id` | string | Unique identifier (e.g. `O01`) |
| `order_name` | string | Human-readable description |
| `complexity` | string | `LOW`, `MEDIUM`, or `HIGH` |
| `required_workers` | int | Number of workers that must work simultaneously |
| `deadline_days_left` | int | Days remaining until the order must be complete |
| `priority` | int | Higher value = higher priority when deadlines tie |
| `total_item_quantity` | int | Total number of items in the order |
| `estimated_total_processing_time_hours` | float | Total work hours needed before team efficiency scaling |
| `requires_heavy_item_lifting` | 0/1 | At least one team member must have this skill |
| `requires_packing` | 0/1 | At least one team member must have this skill |
| `requires_quality_inspection` | 0/1 | At least one team member must have this skill |
| `requires_labeling_before_delivery` | 0/1 | At least one team member must have this skill |
| `test_case_label` | string | Label grouping the order into a test scenario |

### `data/order_items.csv`

Per-item breakdown used for reference and future extensions.

| Column | Type | Description |
|---|---|---|
| `order_id` | string | Foreign key to `orders.csv` |
| `item_name` | string | Item description |
| `item_quantity` | int | Number of units |
| `processing_time_per_item_hours` | float | Hours required per unit |
| `requires_*` | 0/1 | Skill flags inherited from the parent order |

---

## Output

The scheduler prints to stdout.

### Success

```
valid schedule found

day 1, 0:00-2:00 | O06 | duration 2h | team W01(Ahmed)
day 1, 0:00-3:00 | O10 | duration 3h | team W07(Karim)
day 1, 0:00-3:00 | O05 | duration 3h | team W03(Youssef), W09(Hassan)
day 1, 2:00-6:00 | O03 | duration 4h | team W01(Ahmed), W02(Mona)

worker daily hours
W01 Ahmed: day 1=6h
W02 Mona: day 1=4h
...
```

Each schedule line format:

```
day <D>, <START>:00-<END>:00 | <ORDER_ID> | duration <N>h | team <W_ID>(<name>)[, ...]
```

- `day` is 1-indexed.
- Start and end hours are on a 24-hour clock within that day.
- `duration` equals `ceil(estimated_hours / sum_of_team_efficiency_rates)`.

The worker daily hours section lists every worker that was assigned work, showing total hours per day.

### Failure

```
no complete valid schedule found under the hourly backtracking model
```

Exit code is 2 when no schedule exists, 1 on file read error, 0 on success.
