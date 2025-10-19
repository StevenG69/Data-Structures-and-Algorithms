# Data-Structures-and-Algorithms

# Trip Planner Project

## 🎯 Objective

Implement a program `tripPlan.c` that finds **optimal walking and ferry routes** between landmarks in a city, based on user-specified departure times and preferences. The city may be disconnected by foot, but ferry services can bridge otherwise unreachable areas.

The solution must use appropriate **graph data structures** and **graph algorithms**, and handle increasingly complex scenarios across four stages.

## 📥 Input Format

### 1. Landmarks
- Integer `l` (number of landmarks, `l > 0`)
- `l` lines: each contains a landmark name (max 31 chars, no spaces, case-sensitive)

### 2. Walking Links
- Integer `w` (`w ≥ 0`)
- `w` triplets: `landmark1 landmark2 walking_time`  
  (undirected, walking_time > 0)

### 3. Ferry Schedules
- Integer `f` (`f ≥ 0`)
- `f` quadruplets: `from_landmark depart_time to_landmark arrive_time`  
  - Times in `hhmm` 24-hour format (e.g., `0930`)
  - `arrive_time > depart_time`, all within same day

### 4. User Queries
Repeated blocks until `From: done`:

From: <landmark>
To: <landmark>
Departure time: <hhmm>

---

## 🧪 Stages & Requirements

### ✅ Stage 1
- Build a data structure from input.
- Only **direct connections** (walk or ferry) are tested.
- Output the single direct route or `No route.`

### ✅ Stage 2
- Implement **pathfinding on walking-only graph**.
- Input guarantees **exactly one simple path** (no ferries, departure time = `0000`).
- Output full walking itinerary with timestamps.

### ✅ Stage 3
- Support **mixed walking + ferry routes**.
- Must **arrive at ferry departure point on or before ferry departure time**.
- At most **one valid route** exists per query.

### ✅ Stage 4
- Handle **multiple possible routes**.
- Choose the route with **shortest total travel time** (including waiting).
- Guaranteed **unique optimal route**.

---

## 🖨️ Output Format

For each valid route:
```text
Walk <t> minute(s):
<hhmm> <landmark>
<hhmm> <next_landmark>
Ferry <t> minute(s):
<hhmm> <landmark>
<hhmm> <next_landmark>
...

If no valid route:
No route.

On termination:
Happy travels!
