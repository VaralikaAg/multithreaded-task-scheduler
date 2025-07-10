#  Multithreaded Task Scheduler

A modern, efficient **multithreaded task scheduler** written in **C++**, designed for scheduling and execution of real-world system and user tasks with support for **task prioritization**, **execution tracking**, and **interactive task types** like opening system applications. The scheduler dynamically manages tasks and threads.

Inspired by operating system schedulers, the project also includes **turnaround tracking**, **thread-safe logging**, and a **dual-panel Ncurses UI**.

---

## Features

### Task Types
Each task is isolated and runs independently via the thread pool:
- Matrix Multiplication (CPU-Intensive)
- File I/O Operations (File write/read)
- Network Request
- Open Calculator (User-interactive, opens GUI)
- Prime Number Computation
- Disk Usage Reporting (e.g., `df -h`)

Tasks log progress in real time on the left panel.

---

### Task Prioritization
- Tasks can have one of the following priorities: **HIGH**, **MEDIUM**, or **LOW**
- Tasks are stored in a **priority queue**
- The scheduler always selects the **highest priority task** available
- If a task **waits too long**, its priority is automatically increased (aging)

---

### Execution Tracking
- Tracks each task’s:
  - **Start time**
  - **End time**
  - **Execution duration**
  - **Turnaround time**
- This data is displayed in logs for real-time observability

---

### User Interface (Ncurses-Based)

#### **Left Panel**
- Live logs for currently running and completed tasks
- Automatically scrolls as new output appears

#### **Right Panel**
- Interactive task submission menu
- Options to:
  - Choose task type
  - Cancel Pending Task
  - Query status of tasks
- Cursor auto-focuses to input fields

---

## Threaded Architecture

- Backed by a **thread pool** with synchronized access
- Worker threads consume tasks from a shared **priority queue**
- Uses `std::mutex` and `std::condition_variable` for thread safety
- All task states and data are updated live

---

## Technologies & Concepts

### Languages & Libraries
- **C++17 STL**
- **Ncurses** (for UI rendering)

### Key Components

| Component | Description |
|----------|-------------|
| `std::thread`, `std::mutex`, `std::condition_variable` | Thread pool, task signaling, and safe resource access |
| `std::chrono` | Accurate time tracking for execution metrics |
| `std::priority_queue` | Prioritized task dispatching |
| `std::map`, custom `struct` | Metadata management for each task |
| `system()` | Allows GUI app launch (e.g., `gnome-calculator`) |
| Ncurses (`mvprintw`, `getch`, etc.) | Handles dynamic UI rendering and input |

---

## Build & Run

### Prerequisites
- Linux-based OS (native or in a VM)
- `g++` (with C++17 support)
- `ncurses` development library (`libncurses-dev`)

### Build
```bash
make clean && make
```

### Run
```bash
./main
```

---

##  How It Works

1. **Ncurses Setup**: Initializes raw mode, disables line buffering, and sets up dual panes.
2. **Thread Pool Creation**: Spawns fixed worker threads waiting on condition variable.
3. **Menu-Driven Input**: User inputs task type and priority from the right panel.
4. **Task Lifecycle**: Task moves from queue → running → completed. Logs are updated live.
5. **Aging & Re-prioritization**: Older tasks can gain higher priority if delayed too long.
6. **UI Refresh**: `draw_interface()` periodically updates both panels to show real-time info.

---

## Graceful Exit

Press `4` in the right panel menu to:
- Exit the program
- Terminate all running threads cleanly
- Restore terminal to normal state

---