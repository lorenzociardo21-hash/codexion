*This project has been created as part of the 42 curriculum by lciardo.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem, reimagined in a coding context. Multiple coders sit in a circular co-working hub, each needing to grab two USB dongles simultaneously to compile their quantum code. The goal is to coordinate shared resource access using POSIX threads, mutexes, and smart scheduling — preventing deadlocks, starvation, and burnout.

Each coder cycles through three phases: **compiling** (requires both dongles), **debugging**, and **refactoring**. A coder burns out if they go too long without starting a new compilation. The simulation ends either when all coders have compiled enough times, or when one burns out.

Two scheduling policies are supported:
- **FIFO** — dongles are granted to the coder who requested first.
- **EDF (Earliest Deadline First)** — dongles are granted to the coder whose burnout deadline is closest.

---

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` executable. The code compiles with `-Wall -Wextra -Werror -pthread`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

**Arguments:**

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Max ms a coder can go without starting a compile |
| `time_to_compile` | Ms spent compiling (holding both dongles) |
| `time_to_debug` | Ms spent debugging |
| `time_to_refactor` | Ms spent refactoring |
| `number_of_compiles_required` | Compiles each coder must complete to end the sim |
| `dongle_cooldown` | Ms a dongle must rest after being released |
| `scheduler` | `fifo` or `edf` |

**Example:**

```bash
./codexion 4 800 200 200 200 5 100 fifo
```

**Expected output format:**

```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
...
1505 4 burned out
```

---

## Blocking Cases Handled

### Deadlock Prevention
The classic deadlock scenario (each coder holds one dongle and waits forever for the other) is avoided through a **priority queue** embedded in each dongle. Before grabbing a dongle, each coder registers a request in the dongle's queue. A coder only acquires a dongle when it has the highest-priority request in the queue. This means a coder never holds one dongle while indefinitely blocking on the other — the arbitration logic ensures ordered access.

Coffman's four conditions (mutual exclusion, hold-and-wait, no preemption, circular wait) are addressed as follows:
- Mutual exclusion is necessary and kept (dongles are exclusive).
- Hold-and-wait is broken by design: coders register for both dongles via the priority queue before proceeding, so a coder never holds one while blindly waiting for the other.
- Circular wait is eliminated by the total ordering imposed by the scheduler (FIFO timestamps or EDF deadlines), preventing cycles.

### Starvation Prevention
Under **FIFO**, arrival timestamps ensure that waiting coders are served in order. Under **EDF**, the coder closest to burnout always gets priority, which by definition prevents any coder from starving while another with a later deadline keeps taking resources. In tie-breaking, the lower coder ID is preferred to ensure full determinism.

### Cooldown Handling
After a dongle is released, it stores its `last_release_time`. Any coder that acquires it next checks whether `dongle_cooldown` ms have elapsed; if not, it sleeps the remaining time before proceeding. This is handled inside `grab_dongles()` in `routine.c`.

### Precise Burnout Detection
A dedicated **monitor thread** (`controller`) polls all coders at 500 µs intervals, comparing `get_time() - last_compile_start` against `time_to_burnout`. If any coder exceeds the threshold, the burnout is logged immediately and `stop_sim` is set. This guarantees the burnout message appears within 10 ms of the actual event.

### Log Serialization
All `printf` output is protected by `log_mutex`. Before printing, each coder also checks `stop_sim` to avoid printing stale messages after the simulation ends. This ensures lines are never interleaved.

---

## Thread Synchronization Mechanisms

### `pthread_mutex_t` — Three distinct mutexes are used:

- **`dongle->lock`** (one per dongle): Protects the dongle's `flag`, `queue`, and `last_release_time`. Acquired briefly when registering a request or checking the priority queue, and released immediately to allow other threads to proceed.
- **`config->stop_mutex`**: Protects the shared `stop_sim` flag and each coder's `last_compile_start` and `n_compile` counters. Every read or write of these fields is wrapped in lock/unlock to prevent data races between coder threads and the monitor.
- **`config->log_mutex`**: Serializes all `printf` calls so output lines are never interleaved.

### Custom Priority Queue (no `pthread_cond_t` used)
Instead of condition variables, a **spin-wait with `usleep(100)`** is used inside `priority_queue()` in `scheduler.c`. Each dongle holds a two-slot array (`queue[2]`) representing the at-most two coders that can be waiting at once (left and right neighbor). A coder registers its request (ID + priority) in a free slot, then spins until it wins the priority comparison and the dongle's `flag` is 0 (free).

This approach keeps the implementation simple and avoids `pthread_cond_t` complexity, while still being fair and race-free thanks to the dongle's mutex protecting all queue reads/writes.

### Race Condition Prevention — Examples

**`last_compile_start` update:**
```c
pthread_mutex_lock(&coder->config->stop_mutex);
coder->last_compile_start = get_time();
pthread_mutex_unlock(&coder->config->stop_mutex);
```
The monitor reads this field concurrently; the mutex ensures it always sees a consistent value.

**`stop_sim` check in `ft_usleep`:**
```c
while ((get_time() - start) < milliseconds)
{
    pthread_mutex_lock(&coder->config->stop_mutex);
    if (coder->config->stop_sim == 1)
    {
        pthread_mutex_unlock(&coder->config->stop_mutex);
        break;
    }
    pthread_mutex_unlock(&coder->config->stop_mutex);
    usleep(500);
}
```
This makes all sleeping threads responsive to a stop signal within 0.5 ms.

### Thread-Safe Communication Between Coders and Monitor
Coders write to `n_compile` and `last_compile_start` under `stop_mutex`. The monitor thread reads these fields under the same mutex. When the monitor sets `stop_sim = 1`, all coder threads detect it at their next `stop_simu()` check (also under `stop_mutex`), ensuring a clean and synchronized shutdown.

---

## Resources

### Classic References
- [POSIX Threads Programming — Blaise Barney, LLNL](https://hpc-tutorials.llnl.gov/posix/)
- [The Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [`pthread_mutex_t` man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [`gettimeofday` man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

### AI Usage
Claude (Anthropic) was used during this project for the following tasks:
- Generating the `README.md` structure and content based on the project's source code and subject PDF.
- Explaining concurrency concepts (EDF scheduling, Coffman conditions) and how they apply to this specific implementation.
- Reviewing and discussing design choices around the priority queue and the monitor thread pattern.

All AI-generated content was reviewed, understood, and validated before inclusion. The actual C implementation was written independently.
