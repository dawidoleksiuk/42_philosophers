
*This project has been created as part of the 42 curriculum by doleksiu.*

# Philosophers

## Description
The **Philosophers** project is an introduction to the basics of threading a process and exploring the use of mutexes in C. 

The program simulates a classic synchronization problem: one or more philosophers sit at a round table with a large bowl of spaghetti in the middle. The philosophers take turns eating, thinking, and sleeping. To eat, a philosopher must pick up both the fork to their right and the fork to their left. Because there are only as many forks as there are philosophers, the challenge is to design an algorithm that prevents philosophers from starving to death while completely avoiding data races and deadlocks.

This project relies exclusively on threads and mutexes. Each philosopher is represented as a separate thread, and every fork's state is protected with a mutex to prevent duplication and ensure thread safety.

## Instructions

### Compilation
To compile the project, run the provided Makefile from the root of the repository. The source files are compiled using `cc` with the `-Wall`, `-Wextra`, and `-Werror` flags.

```bash
make
```

This will generate the `philo` executable. You can also use `make clean` to remove object files, `make fclean` to remove the executable, and `make re` to recompile the project.

### Execution
The program accepts the following arguments:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

* **number_of_philosophers**: The number of philosophers and also the number of forks.
* **time_to_die** (in milliseconds): If a philosopher has not started eating within `time_to_die` milliseconds since the start of their last meal or the start of the simulation, they die.
* **time_to_eat** (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
* **time_to_sleep** (in milliseconds): The time a philosopher will spend sleeping.
* **number_of_times_each_philosopher_must_eat** *(optional)*: If all philosophers have eaten at least this many times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

### Usage Examples

**Example 1: A stable simulation**
```bash
./philo 5 800 200 200
```
*Starts a simulation with 5 philosophers. They will die if they do not eat within 800ms. Eating takes 200ms, and sleeping takes 200ms. No philosopher should die in this configuration.*

**Example 2: Halting after a specific number of meals**
```bash
./philo 4 410 200 200 5
```
*Starts a simulation with 4 philosophers where each must eat at least 5 times before the program terminates successfully.*

## Implementation Details

### Parsing
Input arguments are validated to ensure they are positive integers. The values are stored in a `t_data` structure. 

### Initialization
Philosophers are stored in a **contiguous array** of structures. Each philosopher is assigned a unique ID and pointers to two mutexes (their own left fork and their neighbor's right fork). 

### Start & Synchronization
All philosopher threads are created in a loop. To prevent deadlocks, a delay strategy is used:
* **Even-numbered** philosophers are delayed by half of the `time_to_eat` at the start of the program.
* If total number of philosophers is odd, a 1ms delay is added to the thinking routine to prevent fork contention and starvation.

### Monitoring & Thread Safety
The main thread (monitor_routine) continuously checks for death or fullness. To ensure thread safety, a dedicated **`mutex_stop`** is used:
* **monitor_routine** locks it to set the `stop_simulation` flag when a death or "everyone is full" condition is met.
* **Philosopher threads** lock it periodically to check the status of the `stop_simulation` flag.
* This mutex is also used to update and read the `eat_count` variable.
This prevents data races and ensures that all threads stop as soon as the simulation is over.

### Cleanup
The program ensures no resources are leaked by:
1. Using `pthread_join` to wait for all threads.
2. Destroying all fork mutexes, the print mutex, and the stop mutex.
3. Freeing the allocated memory for the philosopher array.

## Resources

### Classic References
* [CodeVault - Unix Threads in C (YouTube Playlist)](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2) - An invaluable video series explaining the core concepts of threading, mutexes, and synchronization in C.
* Respective function manual pages and [GeeksforGeeks](https://www.geeksforgeeks.org/c/thread-functions-in-c-c/) explaining these functions.
* [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/) - Comprehensive guide to working with `pthread` libraries in C.
* [Valgrind Helgrind Manual](https://valgrind.org/docs/manual/hg-manual.html) - Documentation for the Valgrind tool used to detect synchronization errors and data races.

### AI Usage
During the development of this project, AI (Google Gemini) was used as an analytical tutor to reduce repetitive troubleshooting and clarify complex OS-level mechanisms. All generated insights were critically assessed. 

Specifically, AI was used for:
* **Log Analysis:** Interpreting terminal output timestamps to distinguish between algorithmic deadlocks and OS-level thread starvation caused by environmental limitations (e.g., WSL terminal I/O blocking).
* **Conceptual Explanations:** Clarifying how AddressSanitizer (`-fsanitize=address`) allocates shadow memory, and how this impacts virtual memory limits and thread scheduling within a WSL environment.
* **Brainstorming Strategies:** Discussing logical concepts to prevent fork contention (e.g., staggering execution times for odd/even threads) without generating or copy-pasting actual C code, ensuring complete personal understanding and responsibility for the final implementation.
* **Documentation:** To generate an initial draft of this README file, which was then manually reviewed, refined, and verified for accuracy against the project specifications.
