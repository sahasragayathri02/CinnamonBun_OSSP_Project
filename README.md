# Linux Process Creation, Execution and Termination System

## 1. Project Title

**Linux Process Creation, Execution and Termination System**

## 2. Project Overview

A terminal-based C application, built for Linux, that demonstrates the
complete lifecycle of a process — creation, execution, identification,
synchronization, and termination — using real POSIX system calls. The
program presents a simple numbered menu; each option isolates one
process-management concept so it can be run and explained independently.

## 3. Problem Statement

Process creation, execution, synchronization and termination are
fundamental operating-system operations, but understanding them
theoretically does not always provide practical understanding of how
Linux actually manages processes. This project provides a small,
hands-on tool that lets a student observe those operations directly —
real PIDs, real parent/child relationships, and real exit statuses —
instead of only reading about them.

## 4. Objectives

01. Understand the complete Linux process lifecycle
02. Create child processes using `fork()`
03. Execute programs using `execvp()`
04. Identify parent and child processes by PID
05. Synchronize processes using `wait()` / `waitpid()`
06. Demonstrate controlled process termination using `exit()`
07. Gain practical Linux/POSIX programming experience

## 5. Proposed Methodology

The application is a single menu loop (`main.c`) that dispatches to five
process-management functions (`process_manager.c`). Each function wraps
exactly one concept:

- `create_child_process()` — calls `fork()` and reports the PID/PPID
  visible from both the parent and the child.
- `execute_program()` — calls `fork()`, then `execvp()` inside the
  child, replacing the child's process image with a user-chosen Linux
  command while the parent keeps running.
- `display_process_info()` — reports the running application's own PID
  and PPID via `getpid()`/`getppid()`.
- `wait_for_child()` — uses `waitpid()` to block the parent until the
  most recently created child terminates, then reports its exit status.
- `terminate_process()` — calls `exit()` with a chosen status code to
  demonstrate controlled termination.

A single file-scope variable, `last_child_pid`, tracks the PID of the
most recently created (but not yet waited-on) child, since this state
needs to persist between separate menu selections.

## 6. Linux / POSIX APIs Used

| System Call | Purpose in this project |
|---|---|
| `fork()` | Creates the child process |
| `execvp()` | Replaces the child's process image with another program |
| `getpid()` | Retrieves the calling process's own PID |
| `getppid()` | Retrieves the calling process's parent PID |
| `wait()` / `waitpid()` | Synchronizes the parent with a terminating child |
| `exit()` / `_exit()` | Terminates a process in a controlled way |

## 7. System Workflow

```
START
  |
  v
Display Menu
  |
  v
User selects an option (1-6)
  |
  +--> 1. fork() -> report parent/child PID & PPID
  +--> 2. fork() + execvp() -> child runs requested command
  +--> 3. getpid() / getppid() -> display process info
  +--> 4. waitpid() -> block until last child exits, report status
  +--> 5. exit(code) -> controlled termination
  +--> 6. exit menu loop cleanly
  |
  v
Return to Menu (unless Exit/Terminate was chosen)
```

## 8. Project Structure

```
LinuxProcessManagement/
│
├── src/
│   ├── main.c                 Menu loop and user interaction
│   ├── process_manager.c      fork/exec/wait/exit implementations
│   └── process_manager.h      Function declarations
│
├── docs/
│   └── abstract.md            Academic abstract
│
├── data/
│   └── README.md              Notes that no external dataset is used
│
├── results/
│   └── README.md              Explains results + a real captured sample run
│
├── reports/
│   └── README.md              Placeholder for final submitted report(s)
│
├── README.md                  This file
├── Makefile                   Build configuration
└── .gitignore
```

## 9. Requirements

- A Linux environment (developed and tested on Ubuntu 24.04)
- GCC (tested with GCC 13.3.0)
- GNU Make
- Git (for version control / GitHub submission)

No external libraries, databases, GUI toolkits, or networking are used.

## 10. Installation / Setup

```bash
git clone <your-repository-url>
cd LinuxProcessManagement
```

(No dependency installation is required beyond `gcc` and `make`, which
ship with most Linux distributions or can be installed with
`sudo apt install build-essential`.)

## 11. Compilation

```bash
make
```

This compiles `src/main.c` and `src/process_manager.c` with
`-Wall -Wextra -std=c11` and links them into an executable named
`process_manager`.

To remove build artifacts:

```bash
make clean
```

## 12. Execution

```bash
./process_manager
```

You will see:

```
========================================
 Linux Process Management System
========================================
1. Create Child Process
2. Execute Program
3. Display Process Information
4. Wait for Child Process
5. Terminate Process
6. Exit
========================================
Enter your choice:
```

Enter a number from 1–6 to run the corresponding operation.

## 13. Testing

The program was actually compiled and run (not just written) in a
Linux/GCC environment during development, and again during a follow-up
code review. The table below reflects tests that were genuinely executed
against the built binary.

| # | Test Case | Method | Result |
|---|---|---|---|
| 1 | Successful `fork()` | Selected option 1; observed two distinct, real PIDs printed (parent and child) | **Passed** |
| 2 | `fork()` failure handling | Not executed — reliably forcing `fork()` to fail requires exhausting system process/resource limits, which was not attempted in this shared environment. The `perror("fork")` error-handling code path exists but was not exercised by an actual failure. | **Not executed** |
| 3 | Correct PID retrieval | Selected option 3; compared printed PID against the process's actual PID | **Passed** |
| 4 | Correct PPID retrieval | Selected option 3; compared printed PPID against the actual parent shell's PID | **Passed** |
| 5 | Successful `execvp()` | Selected option 2, ran `pwd`; child's output appeared and matched the actual working directory | **Passed** |
| 6 | `execvp()` failure handling | Selected option 2, entered a nonexistent command; `execvp` failed, `perror("execvp")` printed, child exited with status 1 | **Passed** |
| 7 | Parent waiting for child | Selected option 4 after option 1/2; parent blocked and printed completion only after the child finished | **Passed** |
| 8 | Child termination | Verified child exit status reported correctly (0 for normal completion, 1 for `execvp` failure) | **Passed** |
| 9 | Parent resuming after child completion | Confirmed "Parent process resumed." prints after "Child process completed." in every run | **Passed** |
| 10 | Invalid menu input | Entered `abc`, `99`, `0`, `-1`; each correctly triggered "Invalid choice." without crashing | **Passed** |
| 11 | Oversized menu input | Piped a 100-character line with no matching command; verified the leftover characters no longer leak into the next prompt (fixed during code review — see Section 17) | **Passed** |
| 12 | Creating a second child before waiting on the first | Selected option 1 twice in a row; verified the program now warns that the first child's PID is no longer tracked, instead of silently dropping it (fixed during code review) | **Passed** |

A full captured transcript of a real run is kept in
[`results/README.md`](results/README.md).

Additional checks performed:
- `make clean` followed by `make` was verified to produce a clean rebuild.
- Compilation was verified to produce **zero warnings** under
  `-Wall -Wextra -std=c11`, and also under the stricter
  `-Wall -Wextra -Wpedantic -std=c11`.
- The source contains no `malloc`/`free` calls, so no dynamic-memory
  leaks are possible (confirmed by inspection; `valgrind` was not
  available in the review environment to additionally confirm at
  runtime).
- End-of-input (EOF on stdin) was tested and exits the program cleanly
  instead of looping forever.

## 14. Expected Outcome

A small, correct, and explainable Linux systems-programming project that
demonstrates — using real system calls and real observed PIDs/exit
statuses — the full lifecycle of a process: creation, execution,
identification, synchronization, and termination.

## 15. Team Members

- Member 1: [Name] – [ID]
- Member 2: [Name] – [ID]
- Member 3: [Name] – [ID]

## 16. Supervisor

[Supervisor Name]

## 17. Project Status

Core implementation complete: all six menu options are implemented,
compiled without warnings, and manually tested (see Section 13). A
follow-up code review (Section 18) found and fixed two robustness
issues. Final report/documentation and team details are pending and
marked with placeholders throughout this repository.

## 18. Code Review Notes

A follow-up review of the initial implementation checked every source
file, the Makefile, and this README for correctness and consistency. Two
real issues were found and fixed:

1. **Oversized input left unread characters on stdin.** If a line typed
   at any prompt was longer than the read buffer, the leftover
   characters used to be picked up as the *next* menu input instead of
   being discarded. Fixed by draining the rest of the line whenever
   `fgets()` doesn't reach a newline (`main.c`, `process_manager.c`).
2. **Creating a second child before waiting on the first silently
   dropped tracking of the first.** Since only one pending child PID is
   tracked at a time, doing this used to leave the first child
   unaccounted for with no indication to the user. Fixed by printing a
   warning when this happens (`create_child_process()`,
   `execute_program()`).

No new menu options, files, or dependencies were added — both fixes are
small, contained changes to existing functions.

## 19. Known Limitations

- **Only one pending child is tracked at a time.** This keeps the
  synchronization logic (and the viva explanation of it) simple, but it
  means creating a second child (option 1 or 2) before waiting on the
  first (option 4) leaves the first child untracked by this program —
  it still runs to completion and is cleaned up by the kernel when this
  program exits, but its exit status is never collected or displayed.
  The program now warns when this happens.
- **`fork()` failure is not exercised in testing** (see Section 13,
  test 2) — only reachable by artificially exhausting system resources,
  which was not attempted. The error-handling code path exists and
  follows the same pattern as the other system calls.
- **No signal handling.** As scoped by the project requirements, the
  program does not use `kill()` or custom signal handlers; termination
  is demonstrated only through `exit()`.

## 20. GitHub Contribution Guidelines

1. Create a feature branch for any change: `git checkout -b feature/<short-name>`
2. Keep commits small and focused (see suggested commit sequence below).
3. Run `make clean && make` and re-test affected menu options before
   opening a pull request.
4. Do not commit build artifacts (`*.o`, the `process_manager` binary) —
   these are already excluded via `.gitignore`.
5. Update this README and `docs/abstract.md` if a change affects the
   project's behaviour or structure.

**Suggested commit sequence:**
1. Initialize project structure
2. Implement process creation
3. Implement process execution
4. Add process synchronization
5. Add process information handling
6. Add process termination
7. Add testing and documentation

*(These are suggested commits for your own history — no commits have
been made on your behalf.)*
