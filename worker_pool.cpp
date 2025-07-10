#include "myheader.h"

vector<thread> worker_threads;
bool stop_workers = false;
mutex stop_mutex;
condition_variable task_available;


void adjust_priorities() {
    lock_guard<mutex> lock(task_map_mutex);
    auto now = chrono::steady_clock::now();

    for (auto& [id, task] : task_map) {
        if (task.status == TaskStatus::PENDING) {
            auto wait_time = chrono::duration_cast<chrono::seconds>(now - task.creation_time).count();
            if (wait_time > 10) { // Aging threshold
                if (task.priority == TaskPriority::LOW) {
                    task.priority = TaskPriority::MEDIUM;
                } else if (task.priority == TaskPriority::MEDIUM) {
                    task.priority = TaskPriority::HIGH;
                }
            }
        }
    }
}


void worker_function() {
    while (true) {
        Task task;

        {
            unique_lock<mutex> lock(queue_mutex);
            task_available.wait(lock, [] { return !task_queue.empty() || stop_workers; });

            if (stop_workers && task_queue.empty()) break;

            task = task_queue.top();
            task_queue.pop();
        }

        // Update status to RUNNING
        auto start_time = chrono::steady_clock::now();
        {
            lock_guard<mutex> lock(task_map_mutex);
            if (task_map[task.task_id].status == TaskStatus::CANCELLED) {
                // log("Skipping cancelled task: " + to_string(task.task_id));
                {
                    std::lock_guard<std::mutex> lock(log_mutex);
                    task_output_logs.push_back("Skipping cancelled task: " + to_string(task.task_id));
                }
                {
                    lock_guard<mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Choice: ");
                    move(p_rows - 2, p_right_start + 8); // place cursor
                    refresh();
                }
                continue;
            }
            task_map[task.task_id].status = TaskStatus::RUNNING;
        }

        ostringstream ss;
        ss << "Worker " << this_thread::get_id() << " is executing Task ID: " << task.task_id;
        // log(ss.str());
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            task_output_logs.push_back(ss.str());
        }
        {
            lock_guard<mutex> lock(interface_mutex);
            draw_interface();
            mvprintw(p_rows - 2, p_right_start, "Choice: ");
            move(p_rows - 2, p_right_start + 8); // place cursor
            refresh();
        }
        
        // Execute Task
        task.func();

        auto end_time = chrono::steady_clock::now();
        auto execution_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

        // Calculate turnaround time
        auto turnaround_time = chrono::duration_cast<chrono::milliseconds>(end_time - task.creation_time).count();

        // Update status to COMPLETED
        {
            lock_guard<mutex> lock(task_map_mutex);
            task_map[task.task_id].status = TaskStatus::COMPLETED;
        }

        ostringstream ss1, ss2, ss3;
        ss1 << "Worker " << this_thread::get_id() << " completed Task ID: " << task.task_id;
        ss2 << "| Execution Time: " << execution_time << " ms";
        ss3 << "| Turnaround Time: " << turnaround_time << " ms";
        // log(ss_done.str());
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            task_output_logs.push_back(ss1.str());
            task_output_logs.push_back(ss2.str());
            task_output_logs.push_back(ss3.str());
        }
        {
            lock_guard<mutex> lock(interface_mutex);
            draw_interface();
            mvprintw(p_rows - 2, p_right_start, "Choice: ");
            move(p_rows - 2, p_right_start + 8); // place cursor
            refresh();
        }
    }
}



void initialize_worker_pool(int thread_count) {
    for (int i = 0; i < thread_count; ++i) {
        worker_threads.emplace_back(worker_function);
    }
    // log(to_string(thread_count) + " worker threads initialized.");
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(to_string(thread_count) + " worker threads initialized.");
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}

void shutdown_worker_pool() {
    {
        lock_guard<mutex> lock(stop_mutex);
        stop_workers = true;
    }

    task_available.notify_all();  // Wake up all worker threads

    for (thread &t : worker_threads) {
        if (t.joinable()) {
            t.join();  // Wait for all threads to finish
        }
    }

    worker_threads.clear();
    // log("Worker pool shut down.");
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back("Worker pool shut down.");
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}
