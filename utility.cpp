#include "myheader.h"
#define posx(x, y) fprintf(stdout, "\033[%d;%dH", x, y)  // Move to (x, y)

// Function to initialize task queue system
void initializeTaskQueue() {
    // Clear task queue and map in case they contain old data
    {
        lock_guard<mutex> lock(queue_mutex);
        while (!task_queue.empty()) {
            task_queue.pop();  // Remove all remaining tasks
        }
    }
    {
        lock_guard<mutex> lock(task_map_mutex);
        task_map.clear();  // Clear task map
    }

    // Nothing else needed for now as priority_queue and unordered_map are ready to use
    // cout << "Task queue initialized.\n";
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back("Task queue initialized.");
    }
}

void cleanupTaskQueue() {
    lock_guard<mutex> lock(queue_mutex);
    while (!task_queue.empty()) {
        task_queue.pop();  // Remove all tasks
    }
    task_map.clear();  // Clear the task status map if exists
}

// Convert TaskStatus enum to string
string taskStatusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::PENDING: return "PENDING";
        case TaskStatus::RUNNING: return "RUNNING";
        case TaskStatus::COMPLETED: return "COMPLETED";
        case TaskStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}


void print_menu() {
    posx(1,50);
    cout << "Task Management System\n";
    posx(2,50);
    cout << "1. Submit Task\n";
    posx(3,50);
    cout << "2. Cancel Task\n";
    posx(4,50);
    cout << "3. Query Task Status\n";
    posx(5,50);
    cout << "4. Exit\n";
    // cout << "Choose an option: ";
}

void print_task_options() {
    posx(7,50);
    cout << "Available Tasks:\n";
    posx(8,50);
    cout << "1. Matrix Multiplication\n";
    posx(9,50);
    cout << "2. File I/O Task\n";
    posx(10,50);
    cout << "3. Network Request Task\n";
    posx(11,50);
    cout << "4. Open Calculator\n";
    posx(12,50);
    cout << "5. Find Prime Numbers\n";  
    posx(13, 50);
    cout<< "6. Disk Space Analysis\n";
    // cout << "Choose a task number: ";
}


// Function to determine priority dynamically
TaskPriority determine_priority(TaskType type) {
    switch (type) {
        case TaskType::CPU_INTENSIVE: return TaskPriority::HIGH;   // Needs quick execution
        case TaskType::IO_BOUND: return TaskPriority::MEDIUM;      // Less critical
        case TaskType::NETWORK: return TaskPriority::MEDIUM;       // Network delays exist
        case TaskType::SYSTEM_MONITOR: return TaskPriority::LOW;   // Can wait
        case TaskType::USER_INTERACTIVE: return TaskPriority::HIGH; // User-driven
        default: return TaskPriority::MEDIUM;
    }
}

string taskPriorityToString(TaskPriority priority) {
    switch (priority) {
        case TaskPriority::HIGH: return "HIGH";
        case TaskPriority::MEDIUM: return "MEDIUM";
        case TaskPriority::LOW: return "LOW";
        default: return "UNKNOWN";
    }
}

void moveCursorTo(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";  // ANSI escape sequence to move cursor
}