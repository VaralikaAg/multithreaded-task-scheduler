#include "myheader.h"

vector<string> task_output_logs;
int scroll_pos = 0;
mutex log_mutex;
mutex interface_mutex;
int p_rows, p_right_start;


void draw_interface() {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int left_width = cols - 22;
    int right_start = left_width + 1;
    p_rows=rows;
    p_right_start=right_start;

    clear();

    // Draw scrollable logs (left)
    {
    lock_guard<mutex> lock(log_mutex);

    int max_log_rows = rows - 2;
    int total_logs = task_output_logs.size();

    int start_index = max(0, total_logs - max_log_rows);

    for (int i = 0; i < max_log_rows && start_index + i < total_logs; ++i) {
        mvprintw(i, 0, "%s", task_output_logs[start_index + i].c_str());
    }
}

    // Draw separator
    for (int i = 0; i < rows; ++i) mvaddch(i, left_width, '|');

    // Draw main menu (right)
    int y = 0;
    mvprintw(y++, right_start, "=== Main Menu ===");
    mvprintw(y++, right_start, "1. Submit Task");
    mvprintw(y++, right_start, "2. Cancel Task");
    mvprintw(y++, right_start, "3. Query Status");
    mvprintw(y++, right_start, "4. Exit");

    y++;
    mvprintw(y++, right_start, "=== Task Types ===");
    mvprintw(y++, right_start, "1. Matrix Mult");
    mvprintw(y++, right_start, "2. File I/O");
    mvprintw(y++, right_start, "3. Network Req");
    mvprintw(y++, right_start, "4. Open Calc");
    mvprintw(y++, right_start, "5. Find Primes");
    mvprintw(y++, right_start, "6. Disk Usage");

    // Input prompt
    // mvprintw(rows - 2, right_start, "Choice: ");
    // move(rows - 2, right_start + 8); // place cursor
    // refresh();
}