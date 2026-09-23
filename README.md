# 🐧 Linux Process Management System

A menu-driven **C-based Linux systems programming project** that demonstrates how operating systems create, execute, identify, synchronize, and terminate processes using real **POSIX system calls**.

---

## 📌 About the Project

Processes are one of the fundamental concepts of an Operating System. Every application running on a computer is managed as one or more processes by the OS.

This project provides a simple terminal-based interface to demonstrate the **complete process lifecycle** in Linux. Instead of only studying process management theoretically, the system allows users to perform and observe these operations using actual process IDs, parent-child relationships, program execution, synchronization, and termination.

---

## 🎯 Objectives

- Understand the Linux process lifecycle
- Create child processes using `fork()`
- Execute external programs using `execvp()`
- Identify processes using PID and PPID
- Synchronize parent and child processes using `wait()` / `waitpid()`
- Demonstrate controlled process termination using `exit()`
- Gain practical experience with Linux/POSIX system calls

---

## ⚙️ Features

| Option | Feature | System Call / Concept |
|---|---|---|
| 1 | Create Child Process | `fork()` |
| 2 | Execute Program | `fork()` + `execvp()` |
| 3 | Display Process Information | `getpid()` + `getppid()` |
| 4 | Wait for Child Process | `waitpid()` |
| 5 | Terminate Process | `exit()` |
| 6 | Exit Application | Program termination |

---

## 🔄 Process Lifecycle

```text
                START
                  │
                  ▼
             Display Menu
                  │
          ┌───────┴────────┐
          │ User Selection │
          └───────┬────────┘
                  │
       ┌──────────┼──────────┐
       ▼          ▼          ▼
     fork()     execvp()   PID/PPID
       │          │          │
       └──────────┼──────────┘
                  │
                  ▼
              waitpid()
                  │
                  ▼
              exit(code)
                  │
                  ▼
                 END
```

---

## 🧠 Core Concepts

### 1. Process Creation — `fork()`

Creates a new child process from the existing parent process.

```text
Parent Process
     │
   fork()
     │
 ┌───┴───┐
 ▼       ▼
Parent  Child
```

The parent and child receive different PIDs, allowing their relationship to be observed.

---

### 2. Program Execution — `execvp()`

The child process can execute an external Linux command using `execvp()`.

For example:

```bash
ls
```

The child process replaces its current program image with the requested command while the parent continues running.

---

### 3. Process Identification

The project displays:

- **PID** — Process ID of the current process
- **PPID** — Process ID of its parent process

Example:

```text
Current Process ID (PID) : 2502
Parent Process ID (PPID) : 2369
```

---

### 4. Process Synchronization — `waitpid()`

The parent process can wait for its child to finish before continuing.

```text
Parent
   │
   │ waitpid()
   ▼
Child completes
   │
   ▼
Parent resumes
```

The child's termination status can also be collected by the parent.

---

### 5. Process Termination — `exit()`

The project demonstrates controlled process termination using an exit status.

An exit status of `0` conventionally indicates successful completion.

---

## 🖥️ Application Menu

```text
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

Each menu option demonstrates a specific operating-system process-management concept.

---

## 🗂️ Project Structure

```text
LinuxProcessManagement/
│
├── src/
│   ├── main.c
│   ├── process_manager.c
│   └── process_manager.h
│
├── docs/
│   └── abstract.md
│
├── data/
│   └── README.md
│
├── results/
│   └── README.md
│
├── reports/
│   └── README.md
│
├── Makefile
├── README.md
└── .gitignore
```

### Source Files

**`main.c`**  
Handles the menu, user input, and program flow.

**`process_manager.c`**  
Contains the implementation of process creation, execution, information display, synchronization, and termination.

**`process_manager.h`**  
Contains function declarations and definitions required by the process-management module.

---

## 🛠️ Technologies Used

- **Language:** C
- **Operating System:** Linux
- **Compiler:** GCC
- **Build Tool:** GNU Make
- **APIs:** POSIX / Linux system calls
- **Version Control:** Git & GitHub

No external libraries, databases, or networking components are required.

---

## 🚀 Getting Started

### Prerequisites

A Linux environment with:

- GCC
- GNU Make
- Git

On Ubuntu, required build tools can be installed with:

```bash
sudo apt install build-essential
```

### Clone the Repository

```bash
git clone <repository-url>
cd LinuxProcessManagement
```

### Compile

```bash
make
```

### Run

```bash
./process_manager
```

### Clean Build Files

```bash
make clean
```

---

## 🧪 Example Execution

### Creating a Child Process

```text
--- Create Child Process ---

Child PID : 2421
Parent PID: 2412
```

### Executing a Program

```text
--- Execute Program ---

Parent PID 2412 launched child PID 2460 to run "ls"
```

### Waiting for a Child

```text
--- Wait for Child Process ---

Parent waiting for child process...
Child process completed.
Child exited normally with status: 0
Parent process resumed.
```

### Displaying Process Information

```text
--- Process Information ---

Current Process ID (PID) : 2502
Parent Process ID (PPID) : 2369
```

### Terminating a Process

```text
--- Process Termination ---

Enter an exit status code (0-255): 0
Terminating with exit status 0...
```

---

## 🌟 Why This Project?

The project converts theoretical Operating Systems concepts into a practical Linux application.

It demonstrates the relationship between:

```text
User
 │
 ▼
Shell
 │
 ▼
C Application
 │
 ▼
POSIX System Calls
 │
 ▼
Linux Kernel
 │
 ▼
Process Management
```

Through this project, concepts such as `fork()`, `execvp()`, `waitpid()`, PID, PPID, and process termination can be observed directly in a real Linux environment.

---

## 👥 Team

| Member | Name | ID |
|---|---|---|
| 1 | V Sameer Reddy | 2520030276 |
| 2 | Ch Sahasra Gayathri | 2520030306 |


### Supervisor

Dr.V. Muniraju Naidu

---

## 📚 Academic Context

This project was developed as part of the **Operating Systems / Systems Programming** coursework to provide practical understanding of Linux process management and POSIX system calls.

---

## 📄 License

This project is intended for **academic and educational purposes**.
