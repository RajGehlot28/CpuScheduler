# CPU Scheduling Analyzer
A C++ simulator I built to benchmark and compare the performance of four core CPU scheduling algorithms:
-   First-Come, First-Served (FCFS)
-   Shortest Job First (SJF) (Preemptive)
-   Priority (Preemptive)
-   Round Robin (RR)

## What It Does
The program takes a list of processes (with arrival time, burst time, and priority) as input.
After running, it calculates and displays the **average waiting time** and **average turnaround time** for each algorithm, allowing for a clear, quantitative comparison of their performance and trade-offs.

## Tech Stack
-   **Language:** C++
-   **Core Concepts:** Object-Oriented Programming (OOP)
-   **Data Structures:** C++ Standard Library (STL), primarily `std::queue` for the ready queue.
