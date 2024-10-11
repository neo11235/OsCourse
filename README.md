# OsCourse
Operating System course assignments.

## Assignment 1
Write a bash script that evaluates each student’s script, compares the
output with an expected output text, and grades the script accordingly.

## Assignment 2

### 1. Implement a new system call `trace`
Implement a system call named `trace` that will trace a program's system calls.

### 2. Implement a system call `sysinfo`
Implement a system call named `sysinfo` that provides information about the current running system. When called from a user program, the system call should output the following:

- **Free memory**: The amount of free memory available in the system (in bytes).
- **Number of processes**: The total number of existing processes in the current system.


## Assignment 3
Implement a scheduler that uses lottery scheduling algorithm

[xv6 scheduling](https://github.com/Tahmid04/xv6-scheduling-july-2022?tab=readme-ov-file)

## Assignment 4
Implement locks, semaphores etc synchronization premitives.


## Assignment 5

### 1. Copy-On-Write (COW)
Implement Copy-On-Write (COW) for the `fork()` system call. In the default behavior of `fork()` in xv6, all user-space memory of the parent process is copied to the child. COW optimizes this by delaying the copy of pages until a write operation occurs, minimizing unnecessary duplication of memory. When a page is written to, a page fault is triggered, and the kernel allocates a new page for the faulting process, copying the data from the original page.


### 2. Paging
Implement paging in the system to handle memory management when physical memory is full. Paging moves some pages to persistent storage (swap space) and brings them back when needed, allowing the system to use more virtual memory than physical memory. The decision of which pages to swap is based on a page replacement algorithm.
