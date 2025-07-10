#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <map>
#include <chrono>
#include <random>
#include <fstream>
#include <ncurses.h>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <atomic>

using namespace std;

enum class TaskPriority { HIGH, MEDIUM, LOW };
enum class TaskStatus { PENDING, RUNNING, COMPLETED, CANCELLED };
enum class TaskType {
    CPU_INTENSIVE,
    IO_BOUND,
    NETWORK,
    SYSTEM_MONITOR,
    USER_INTERACTIVE
};


// Structures
struct Task {
    int task_id;                   // Unique Task ID
    function<void()> func;          // Function pointer for the task
    TaskPriority priority;          // Priority level (can change dynamically)
    TaskStatus status;              // Current status
    TaskType type;                  // Task type (SYSTEM, CPU_INTENSIVE, etc.)
    chrono::steady_clock::time_point creation_time;  // Timestamp when task was submitted
    int execution_time;
};

struct TaskComparator {
    bool operator()(const Task& a, const Task& b) {
        // Higher priority tasks should come first
        if (a.priority != b.priority)
            return a.priority < b.priority;  // Higher priority first

        // If priority is same, use aging (older tasks first)
        return a.creation_time > b.creation_time;
    }
};

// Variables
extern priority_queue<Task, vector<Task>, TaskComparator> task_queue; 
extern mutex queue_mutex;
extern condition_variable task_available;
extern unordered_map<int, Task> task_map;
extern mutex task_map_mutex;
extern vector<string> task_output_logs;
extern mutex log_mutex;
extern mutex interface_mutex;
extern int scroll_pos;
extern int p_rows, p_right_start;


// Common functions used across the project
void initializeTaskQueue();
int submitTask(function<void()> func, TaskType type);
int cancelTask(int taskId);
TaskStatus queryTaskStatus(int taskId);
string taskStatusToString(TaskStatus status);
void cleanupTaskQueue();
void initialize_worker_pool(int thread_count);
void shutdown_worker_pool();
void worker_function();
void enqueue_task(const Task& task);
bool is_queue_empty();
int get_queue_size();
void print_menu();
void print_task_options();
void adjust_priorities();
TaskPriority determine_priority(TaskType type);
string taskPriorityToString(TaskPriority priority);
void moveCursorTo(int row, int col);
void draw_interface();




void find_primes_task();
void network_request_task();
void file_io_task();
void open_calculator_task();
void matrix_multiplication_task();
void disk_space_analysis_task();