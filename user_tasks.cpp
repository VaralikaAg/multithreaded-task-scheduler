#include "myheader.h"

void find_primes_task() {
    const int LIMIT = 1000000;
    vector<bool> is_prime(LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;

    ostringstream oss;
    oss << "Finding prime numbers...Thread: " << this_thread::get_id();
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }

    for (int i = 2; i * i <= LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    oss.str("");  // Clear the contents
    oss.clear();  // Reset the state of the stream
    oss << "Prime number calculation completed.Thread: " << this_thread::get_id() ;
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}


void network_request_task() {
    ostringstream oss;
    oss << "Starting network request...Thread: " << this_thread::get_id();
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }

    #ifdef _WIN32
        system("curl -s https://www.google.com > nul"); // Windows (Suppress output)
    #else
        system("curl -s https://www.google.com > /dev/null"); // Linux/macOS
    #endif

    oss.str("");  // Clear the contents
    oss.clear();  // Reset the state of the stream
    oss << "Network request completed.Thread: " << this_thread::get_id();
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}



void file_io_task() {
    ostringstream oss;
    oss << "Writing 1GB of data to file...";
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
    ofstream file("large_file.txt", ios::binary);
    vector<char> buffer(1024 * 1024, 'A');  // 1MB buffer

    for (int i = 0; i < 1024; ++i) {  // Write 1024MB = 1GB
        file.write(buffer.data(), buffer.size());
    }
    file.close();
    oss.str("");  // Clear the contents
    oss.clear();  // Reset the state of the stream
    oss << "File write completed. Now reading";
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }

    ifstream file_read("large_file.txt", ios::binary);
    while (file_read.read(buffer.data(), buffer.size()));
    file_read.close();
    oss.str("");  // Clear the contents
    oss.clear();  // Reset the state of the stream
    oss << "File Read Completed!";
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}



void matrix_multiplication_task() {
    ostringstream oss;
    const int SIZE = 1000;
    vector<vector<int>> A(SIZE, vector<int>(SIZE));  // Matrix A
    vector<vector<int>> B(SIZE, vector<int>(SIZE));  // Matrix B
    vector<vector<int>> C(SIZE, vector<int>(SIZE, 0));  // Result matrix

    // Random number generator
    random_device rd;
    mt19937 gen(rd());  
    uniform_int_distribution<int> dist(1, 10);  // Random numbers between 1 and 10

    // Fill matrices A and B with random values
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
    }

    oss << "Starting matrix multiplication.Thread: " << this_thread::get_id();
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }

    // Matrix multiplication
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    oss.str("");  // Clear the contents
    oss.clear();  // Reset the state of the stream
    oss << "Matrix multiplication completed!Thread: " << this_thread::get_id();
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}

void open_calculator_task() {
    ostringstream oss;

    oss << "Opening calculator.Thread: " << this_thread::get_id();
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }

    #ifdef _WIN32
        system("calc"); // Windows command to open calculator
    #elif __APPLE__
        system("open -a Calculator"); // macOS command
    #elif __linux__
        system("gnome-calculator"); // Linux (GNOME-based)
    #endif

    oss.str("");  // Clear the contents
    oss.clear();  // Reset the state of the stream
    oss << "Closing calculator.Thread: " << this_thread::get_id();
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}

void disk_space_analysis_task() {
    ostringstream oss;
    oss << "Analyzing disk space usage...";
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
    system("df -h > disk_usage.txt");  // Get human-readable disk usage
    oss.str("");  // Clear the contents
    oss.clear();  // Reset the state of the stream
    oss << "Disk space analysis saved!";
    // log(oss.str());
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        task_output_logs.push_back(oss.str());
    }
    {
        lock_guard<mutex> lock(interface_mutex);
        draw_interface();
        mvprintw(p_rows - 2, p_right_start, "Choice: ");
        move(p_rows - 2, p_right_start + 8); // place cursor
        refresh();
    }
}
