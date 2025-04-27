# CPU-scheduling-policies
C program to simulate cpu scheduling policies
CPU Scheduling Policies Simulator
Project Overview
This project implements and visualizes several CPU scheduling algorithms using C/C++. It was developed as part of the Operating Systems (CC373) course at Alexandria University, Faculty of Engineering.

The simulator can operate in two modes:

Trace mode: Displays a visual timeline of process scheduling.

Stats mode: Outputs detailed statistics about process execution.

Implemented Scheduling Algorithms
FCFS (First Come First Serve)

RR (Round Robin)

SPN (Shortest Process Next)

SRT (Shortest Remaining Time)

HRRN (Highest Response Ratio Next)

FB-1 (Feedback Queue with quantum = 1)

FB-2i (Feedback Queue with quantum = 2^i)

Aging (Priority Aging to prevent starvation)

Features
Input through stdin and output through stdout.

Makefile provided for easy compilation.

Strict adherence to output format for automatic grading.

Bonus: Optional Dockerfile included for containerized builds.

Ready for redirection and piping input/output with Linux commands.
