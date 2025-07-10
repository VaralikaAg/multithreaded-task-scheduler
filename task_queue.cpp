#include "myheader.h"

priority_queue<Task, vector<Task>, TaskComparator> task_queue;
mutex queue_mutex;

void enqueue_task(const Task& task) {
    lock_guard<mutex> lock(queue_mutex);

    // Push task into a priority queue (sorted order)
    task_queue.push(task);  // Ensure priority-based ordering
    // cout << "Task " << task.task_id << " added to queue.\nPriority: " << taskPriorityToString(task.priority) << "\n";
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back("Task " + to_string(task.task_id) + " added to queue.");
        task_output_logs.push_back("Priority: " + taskPriorityToString(task.priority));
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }

    task_available.notify_one();
}

bool is_queue_empty() {
    lock_guard<mutex> lock(queue_mutex);
    return task_queue.empty();
}

int get_queue_size() {
    lock_guard<mutex> lock(queue_mutex);
    return task_queue.size();
}
