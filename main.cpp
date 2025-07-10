#include "myheader.h"
#define posx(x, y) fprintf(stdout, "\033[%d;%dH", x, y)  // Move to (x, y)



int main() {
    srand(time(0));
    int choice;
    initializeTaskQueue();
    initialize_worker_pool(4);  // Assuming 4 worker threads
    
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    while (true) {
        
        {
            lock_guard<mutex> lock(interface_mutex);
            draw_interface();
            mvprintw(p_rows - 2, p_right_start, "Choice: ");
            move(p_rows - 2, p_right_start + 8); // place cursor
            refresh();
        }


        char input[10];
        echo();
        getnstr(input, sizeof(input) - 1);
        noecho();

        choice = atoi(input);

        switch (choice) {
            case 1: {  // Submit Task
                {
                    std::lock_guard<std::mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Task Type (1-6): ");
                    move(p_rows - 2, p_right_start + 17); // place cursor
                    refresh();
                }
                echo(); getnstr(input, 4); noecho();
                int task_choice = atoi(input);
                

                TaskType task_type;
                function<void()> task_func;

                switch (task_choice) {
                    case 1: task_func = matrix_multiplication_task; task_type = TaskType::CPU_INTENSIVE; break;
                    case 2: task_func = file_io_task; task_type = TaskType::IO_BOUND; break;
                    case 3: task_func = network_request_task; task_type = TaskType::NETWORK; break;
                    case 4: task_func = open_calculator_task; task_type = TaskType::USER_INTERACTIVE; break;
                    case 5: task_func = find_primes_task; task_type = TaskType::CPU_INTENSIVE; break;
                    case 6: task_func = disk_space_analysis_task; task_type = TaskType::SYSTEM_MONITOR; break;


                    default:
                    {
                        std::lock_guard<std::mutex> lock(log_mutex);
                        task_output_logs.push_back("Invalid task choice.");
                    }
                        continue;
                }

                // Submit task with auto-priority assignment
                int id = submitTask(task_func, task_type);
                {
                    std::lock_guard<std::mutex> lock(log_mutex);
                    task_output_logs.push_back("Task submitted with ID: " + to_string(id));
                }
                {
                    lock_guard<mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Choice: ");
                    move(p_rows - 2, p_right_start + 8); // place cursor
                    refresh();
                }
                // log("Task submitted with ID: " + to_string(id));
                break;
            }

            case 2:  // Cancel Task
            {
                {
                    lock_guard<mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Cancel Task ID: ");
                    move(p_rows - 2, p_right_start + 16); // place cursor
                    refresh();
                }
                echo(); getnstr(input, 5); noecho();
                int id = atoi(input);
                if (cancelTask(id) == 0)
                    // log("Task " + to_string(id) + " cancelled.");
                    {
                        std::lock_guard<std::mutex> lock(log_mutex);
                        task_output_logs.push_back("Task " + to_string(id) + " cancelled.");
                    }
                else
                    // log("Failed to cancel task " + to_string(id));
                    {
                        std::lock_guard<std::mutex> lock(log_mutex);
                        task_output_logs.push_back("Failed to cancel task " + to_string(id));
                    }
                {
                    lock_guard<mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Choice: ");
                    move(p_rows - 2, p_right_start + 8); // place cursor
                    refresh();
                }
                break;
            }

            case 3:  // Query Task Status
            {
                {
                    lock_guard<mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Query Task ID: ");
                    move(p_rows - 2, p_right_start + 15); // place cursor
                    refresh();
                }
                echo(); getnstr(input, 5); noecho();
                int id = atoi(input);
                TaskStatus status = queryTaskStatus(id);
                // log("Task " + to_string(id) + " is " + taskStatusToString(status));
                {
                    std::lock_guard<std::mutex> lock(log_mutex);
                    task_output_logs.push_back("Task " + to_string(id) + " is " + taskStatusToString(status));
                }
                {
                    lock_guard<mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Choice: ");
                    move(p_rows - 2, p_right_start + 8); // place cursor
                    refresh();
                }
                break;
            }

            case 4:  // Exit
                shutdown_worker_pool();
                cleanupTaskQueue();
                endwin();
                break;

            default:
                // log("Invalid menu choice.");
                {
                    std::lock_guard<std::mutex> lock(log_mutex);
                    task_output_logs.push_back("Invalid menu choice.");
                }
                {
                    lock_guard<mutex> lock(interface_mutex);
                    draw_interface();
                    mvprintw(p_rows - 2, p_right_start, "Choice: ");
                    move(p_rows - 2, p_right_start + 8); // place cursor
                    refresh();
                }
                break;

        }
    }

    return 0;
}
