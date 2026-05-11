# student a2 algorithm design and analysis

## formal problem abstraction

We model the warehouse as a constrained scheduling problem.

- each order has an id, required skills, required number of workers, estimated processing hours, priority, and a deadline measured in days.
- each worker has skills, regular daily hours, maximum overtime hours, efficiency rate, and hourly cost.
- time is divided into 1-hour slots. day 0 is today, slot 0 is the first hour of the first day.
- a scheduled assignment is `(order, team, day, start hour, end hour)`.
- for team orders, all selected workers must work on the same order during the exact same interval.
- the duration of an order for a selected team is `ceil(order estimated hours / sum of team efficiency rates)`.

The output is a schedule that assigns every order to a valid team and time interval before its deadline. If this is not possible, the algorithm reports failure.

## industrial considerations

In a real warehouse, the schedule should avoid unsafe heavy-item handling, keep certified workers on the right tasks, and avoid overusing the same workers every day. The model also keeps overtime limited because workers can have overtime capacity, but too much overtime is still not preferred. exact start and end times are important because the baseline daily-hour matrix can say a worker has enough total hours while still accidentally assigning them to two tasks at the same time.

## baseline algorithm

The baseline from a1 uses a greedy method. it sorts orders by urgency, then assigns suitable workers based on skills and available daily hours. the output is a 3d vector:

`dayWorkerOrderHours[day][worker][order]`

This is useful as a first solution because it checks skills and daily capacity. however, it is limited because it only stores total hours per day. it does not prove exact start and end times, so it cannot fully guarantee that a worker is not double-booked inside the same day. it also cannot fully prove that a team worked simultaneously on a heavy order.

## improved algorithm

The improved algorithm uses plain backtracking. it schedules the unscheduled order with the earliest deadline, tries every feasible worker team for that order, tries possible start times before the deadline, and recursively continues. if a later order cannot be scheduled, the algorithm removes the last placement and tries another team or time.

hard constraints:

- the team must cover all required skills.
- the team size must equal the order's required worker count.
- all team members must be free for the entire selected interval.
- all team members must stay within regular hours plus maximum overtime for that day.
- the order must finish before its deadline.
- multi-worker orders must use the same start and end time for the whole team.

objective priority:

1. schedule all orders before their deadlines.
2. prefer earlier completion times by trying placements with earlier finish times first.
3. prefer less overtime and lower cost when ordering placement choices.

## pseudocode

```text
backtracking_scheduler(workers, orders):
    sort orders by earliest deadline, then higher priority
    create worker_slot_busy[worker][time_slot] = false
    create worker_daily_hours[worker][day] = 0
    schedule = empty list

    if schedule_from(0) is true:
        return schedule
    else:
        return failure

schedule_from(order_index):
    if order_index == number of orders:
        return true

    order = sorted_orders[order_index]
    teams = all worker teams with size order.required_workers
    remove teams that do not cover order.required_skills
    candidates = empty list

    for each team in teams:
        duration = ceil(order.estimated_hours / sum(team efficiency))

        for each day before order.deadline:
            for each start_hour where start_hour + duration <= 24:
                start_slot = day * 24 + start_hour
                end_slot = start_slot + duration

                if end_slot is after order.deadline:
                    continue

                if every worker in team is free for all slots
                   and daily hours stay within regular + overtime:
                    add (team, day, start_hour, duration) to candidates

    sort candidates by:
        earlier finish time,
        lower added overtime,
        lower team cost,
        higher team efficiency

    for each candidate in candidates:
        place the assignment

        if schedule_from(order_index + 1) is true:
            return true

        remove the assignment

    return false
```

## correctness argument

The algorithm only places an order after checking every hard constraint. the team coverage check guarantees that the required skills are present. The team-size check guarantees that heavy or complex orders receive the required number of workers. The slot availability table guarantees that a worker cannot be assigned to two orders in the same hour. The daily-hours table guarantees that a worker does not exceed regular hours plus allowed overtime. The deadline check guarantees that every accepted order finishes before its deadline.

For multi-worker orders, the algorithm places the whole team using one shared start slot and one shared end slot, so the workers are simultaneous. This fixes the main limitation in the baseline.

The recursive search tries possible valid teams and possible valid start times for each order. When a choice later blocks the schedule, backtracking removes that choice and tries another. Therefore, if the algorithm returns a schedule, that schedule satisfies all hard constraints. If it reports failure, it means no schedule was found under the chosen hourly-slot model and search rules.

## complexity analysis

let:

- `O` be the number of orders.
- `W` be the number of workers.
- `T` be the number of hourly time slots.
- `R` be the maximum required team size.
- `K` be the maximum number of feasible teams for one order, where `K <= C(W, R)`.

For each order, the algorithm may try up to `K` teams and up to `T` start slots. In the worst case, backtracking can explore about:

`O((K * T)^O)`

states. Each feasibility check scans the workers in the team and the slots in the order duration, so the real cost also includes a factor around `O(R * duration)`.

space complexity is:

`O(W * T + O)`

For the worker availability table, daily-hours table, recursion stack, and final schedule.

## examples and edge cases

- simple low-complexity order: one worker with packing, inspection, and labeling skills can be assigned alone.
- heavy order: a high-complexity order with `required_workers = 3` must get three workers at the exact same interval.
- tight deadline: an order due in one day is scheduled before later-deadline orders because earliest deadline is used.
- limited skills: if an order needs heavy lifting, a team with no heavy-lifting worker is rejected.
- overlap conflict: if a worker is already busy from 9 to 12, another order cannot use that worker in any of those slots.
- overtime boundary: a worker with 8 regular hours and 2 overtime hours can work up to 10 hours that day, but not 11.
- failure case: if no valid team and time interval exists before the deadline, the algorithm reports failure.

## implemented solution

The implementation is in `scheduler.cpp`. it reads `workers.csv` and `orders.csv`, builds feasible teams, schedules orders using backtracking, and prints either a valid schedule or a failure message.

to compile and run:

```bash
g++ -std=c++17 scheduler.cpp -o scheduler
./scheduler
```

## human development notes

- i first looked at the a1 baseline and it made sense as a greedy starting point.
- the main thing i noticed was that daily total hours are not enough for a real schedule, because the same worker could still overlap inside the day.
- i thought about just improving the greedy sorting, but that would still not really fix the exact-time problem.
- i picked backtracking because it lets me try a team and time, then undo it if later orders get stuck.
- i used 1-hour slots so the algorithm stays understandable and not too complicated.
- i made team orders use the same interval because if workers are carrying or packing heavy furniture together, they need to be there together not just on the same day.
- i kept overtime allowed but bounded, because the dataset already has overtime values.

## time log

| task | rough time |
| --- | ---: |
| read a1 files and datasets | 10 min |
| identify baseline limitation | 10 min |
| choose backtracking and exact slots | 10 min |
| write pseudocode and constraints | 40 min |
| write correctness and complexity | 40 min |
| prepare implementation and quick run | 30 min |
| total | 2+ hours |
