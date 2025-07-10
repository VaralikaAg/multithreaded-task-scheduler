#include "myheader.h"

// Global map to keep track of task statuses by ID
unordered_map<int, Task> task_map; 
mutex task_map_mutex;

// Global task ID generator
int task_id_counter = 0;
mutex task_id_mutex;

// ---- Function to Generate Unique Task ID ----
int generate_task_id() {
    lock_guard<mutex> lock(task_id_mutex);
    return ++task_id_counter;
}

// ---- Submit a New Task ----
int submitTask(function<void()> func, TaskType type) {
    int id = generate_task_id();  // Generate unique ID for the task

    TaskPriority priority = determine_priority(type);  // Auto-assign priority

    // Create new Task object
    Task new_task;
    new_task.task_id = id;
    new_task.priority = priority;
    new_task.func = func;
    new_task.status = TaskStatus::PENDING;
    new_task.creation_time = chrono::steady_clock::now();  // Track aging

    {
        // Add task to map for tracking
        lock_guard<mutex> lock(task_map_mutex);
        task_map[id] = new_task;
    }

    // Add task to queue for execution
    enqueue_task(new_task);
    task_available.notify_one();  // Notify a worker thread

    return id;  // Return the generated task ID
}



// ---- Cancel a Task ----
int cancelTask(int task_id) {
    lock_guard<mutex> lock(task_map_mutex);

    // Check if task ID exists in map
    auto it = task_map.find(task_id);
    if (it != task_map.end()) {
        Task &task = it->second;

        // Only allow cancellation if task is still pending
        if (task.status == TaskStatus::PENDING) {
            task.status = TaskStatus::CANCELLED;
            return 0;  // Success
        } else {
            return -1;  // Cannot cancel if running or completed
        }
    }
    return -1;  // Task ID not found
}

// ---- Query Task Status ----
TaskStatus queryTaskStatus(int task_id) {
    lock_guard<mutex> lock(task_map_mutex);

    // Check if task ID exists in map
    auto it = task_map.find(task_id);
    if (it != task_map.end()) {
        // cout << "Task " << task_id << " status: " << taskStatusToString(it->second.status)
        //      << ", Priority: " << taskPriorityToString(it->second.priority) << endl;
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            task_output_logs.push_back("Task " + to_string(task_id) + " status: " + taskStatusToString(it->second.status));
            task_output_logs.push_back("Priority: " + taskPriorityToString(it->second.priority));
        }
        {
            lock_guard<mutex> lock(interface_mutex);
            draw_interface();
            mvprintw(p_rows - 2, p_right_start, "Choice: ");
            move(p_rows - 2, p_right_start + 8); // place cursor
            refresh();
        }
        return it->second.status;
    }

    // cout << "Task " << task_id << " not found. Returning CANCELLED.\n";
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back("Task " + to_string(task_id)+ " not found. Returning CANCELLED.");
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
    return TaskStatus::CANCELLED;  // If task not found, return CANCELLED as error indicator
}

