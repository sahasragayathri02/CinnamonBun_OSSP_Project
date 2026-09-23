/*
 * process_manager.h
 *
 * Declares the process-management operations used by the menu-driven
 * application in main.c. Each function corresponds to one menu option
 * and wraps a small, focused demonstration of a POSIX process API.
 */

#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

/* Creates a child process with fork() and reports parent/child PIDs. */
void create_child_process(void);

/* Forks a child that replaces its process image using execvp(). */
void execute_program(void);

/* Displays the current process's PID and its parent's PID (PPID). */
void display_process_info(void);

/* Waits for the most recently created child to finish (wait/waitpid). */
void wait_for_child(void);

/* Demonstrates controlled process termination using exit(). */
void terminate_process(void);

#endif /* PROCESS_MANAGER_H */
