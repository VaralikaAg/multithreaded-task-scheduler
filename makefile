# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -pthread -g
LIBS = -lncurses

# Header dependencies
DEPS = myheader.h

# Object files
OBJ = main.o user_tasks.o task_queue.o worker_pool.o task_manager.o utility.o interface.o

# Build rules
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile the final executable
main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	rm -rf *.o

# Clean build artifacts
clean:
	rm -rf *.o main
