# Abstract — Linux Process Creation, Execution and Termination System

## 1. Abstract

This project implements a terminal-based application that demonstrates
the complete lifecycle of a process on a Linux system: creation,
execution, identification, synchronization, and termination. Rather than
studying process management purely in theory, the project uses real
POSIX system calls — `fork()`, `execvp()`, `getpid()`, `getppid()`,
`wait()`/`waitpid()`, and `exit()` — so that each concept can be observed
directly through actual kernel behaviour rather than a simulation.

## 2. Problem Statement

Process creation, execution, synchronization and termination are
fundamental operating-system operations, but understanding them
theoretically does not always provide practical understanding of how
Linux actually manages processes. Students can typically define `fork()`
or `wait()` in an exam answer without ever having watched a parent and
child process run side by side, or seen an exit status propagate back to
a waiting parent. This project addresses that gap with a small, hands-on
tool built around those exact system calls.

## 3. Objectives

- Understand the complete Linux process lifecycle
- Create child processes using `fork()`
- Execute programs using `execvp()`
- Identify parent and child processes via their PIDs
- Synchronize processes using `wait()` / `waitpid()`
- Demonstrate controlled process termination using `exit()`
- Gain practical Linux/POSIX systems-programming experience

## 4. Proposed Methodology

A single C application presents a numbered terminal menu. Each menu
option isolates one concept:

1. **Create Child Process** — calls `fork()` and prints the PID/PPID
   seen by each of the two resulting processes.
2. **Execute Program** — calls `fork()` followed by `execvp()` in the
   child, so the child's process image is replaced by a user-specified
   Linux command while the parent (this menu program) keeps running.
3. **Display Process Information** — reports the running application's
   own PID and PPID via `getpid()`/`getppid()`.
4. **Wait for Child Process** — uses `waitpid()` so the parent blocks
   until the most recently created child finishes, then reports its
   exit status.
5. **Terminate Process** — calls `exit()` with a user-chosen status code
   to demonstrate a controlled shutdown.

The implementation is kept intentionally small and modular
(`main.c` for the menu, `process_manager.c`/`.h` for the process logic)
so that each function maps directly onto one system call and one
academic concept.

## 5. Operating Systems Concepts / Linux APIs Used

| Concept | API |
|---|---|
| Process creation | `fork()` |
| Process execution | `execvp()` |
| Process identification | `getpid()`, `getppid()` |
| Process synchronization | `wait()`, `waitpid()` |
| Process termination | `exit()`, `_exit()` |

## 6. Individual Contribution

- [Member 1 Name] — [ID]: [Contribution — e.g. process creation module]
- [Member 2 Name] — [ID]: [Contribution — e.g. execution module]
- [Member 3 Name] — [ID]: [Contribution — e.g. synchronization, testing]

*(Placeholders — replace with actual team contributions.)*

## 7. Tools / Platforms / Software Used

- **Language:** C (C11)
- **Platform:** Linux (developed and tested on Ubuntu 24.04, Linux
  kernel 6.x)
- **Compiler:** GCC 13.3.0
- **Build system:** GNU Make
- **Version control:** Git / GitHub

## 8. Expected Outcome

A working, self-contained command-line tool that a student can run,
explain line-by-line, and use to demonstrate — with real, observable PID
values and exit statuses rather than invented ones — how Linux creates,
runs, synchronizes, and terminates processes.
