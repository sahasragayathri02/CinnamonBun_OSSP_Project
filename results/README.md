# Results

This project produces no persistent output files during normal use — its
"results" are the terminal output produced while running the menu-driven
application. This directory is where such output can be captured and kept
for reference (e.g. before a viva or demo).

## What belongs here

- Terminal transcripts captured with a command such as:
  ```
  ./process_manager > results/sample_run.txt 2>&1
  ```
- Notes on which menu options were exercised in a given run.

## Sample run (captured on the development machine)

The transcript below is real output captured by actually compiling and
running `process_manager` in a Linux/GCC environment (Ubuntu 24.04,
GCC 13.3.0) during development, feeding it the choices `3, 1, 4, 2 (pwd),
4, 5 (0)`. It has not been edited or invented.

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
--- Process Information ---
Current Process ID (PID) : 183
Parent Process ID (PPID) : 181

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
--- Create Child Process ---
Parent Process
Parent PID: 183
Child PID created: 184

Note: the child has not been waited on yet.
Use option 4 (Wait for Child Process) to synchronize with it.

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
--- Wait for Child Process ---
Parent waiting for child process...
Child process completed.
Child exited normally with status: 0
Parent process resumed.

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
--- Execute Program ---
Enter a command to run (e.g. ls -l), or press Enter for "ls": /home/claude/LinuxProcessManagement
Parent PID 183 launched child PID 185 to run "pwd"

Note: the child has not been waited on yet.
Use option 4 (Wait for Child Process) to synchronize with it.

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
--- Wait for Child Process ---
Parent waiting for child process...
Child process completed.
Child exited normally with status: 0
Parent process resumed.

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
--- Process Termination ---
Enter an exit status code (0-255): Terminating with exit status 0...
```

Note: the PID values above (183, 184, 185, ...) are specific to that one
run. Every execution will show different, real PID values assigned by the
Linux kernel at that time — they are never hard-coded.

No performance metrics, benchmarks, or fabricated data are included here.
