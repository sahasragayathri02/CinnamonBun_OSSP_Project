/*
 * process_manager.c
 *
 * Implements process creation, execution, identification,
 * synchronization and termination using standard POSIX/Linux
 * system calls: fork(), execvp(), getpid(), getppid(), wait(),
 * waitpid() and exit().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "process_manager.h"

#define MAX_INPUT_LEN 256
#define MAX_ARGS      32

/*
 * last_child_pid tracks the PID of the most recently created child that
 * has not yet been waited on. It is the one piece of state that needs to
 * persist between separate menu selections (e.g. "Create Child Process"
 * on one call, then "Wait for Child Process" on a later call), so a
 * single file-scope variable is used instead of passing state through
 * the menu loop. A value <= 0 means no child is currently pending.
 */
static pid_t last_child_pid = -1;

/*
 * If fgets() filled the buffer without reaching a newline, the rest of
 * that input line is still sitting on stdin. This drains it so leftover
 * characters aren't misread as input for a later menu option.
 */
static void discard_rest_of_line(const char *buf)
{
    if (strchr(buf, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* discard */
        }
    }
}

/* ------------------------------------------------------------------ */
/* 1. Process creation                                                */
/* ------------------------------------------------------------------ */
void create_child_process(void)
{
    printf("\n--- Create Child Process ---\n");

    if (last_child_pid > 0) {
        /* This program only tracks one pending child at a time. Creating
         * another before waiting on the first means the first's PID is
         * no longer tracked here - it will keep running (or, if it has
         * already finished, sit as a zombie) until this program exits,
         * at which point the kernel reparents it to init and reclaims
         * it. Warn rather than silently losing track of it. */
        printf("Warning: child PID %d was never waited on and is no longer tracked.\n",
               last_child_pid);
    }

    /* Flush stdout before fork() so buffered parent output is written
     * out before the child (which inherits a copy of the same buffer)
     * has a chance to add its own output out of order. This matters
     * whenever stdout is not a terminal, e.g. when piped or redirected. */
    fflush(stdout);

    pid_t pid = fork();

    if (pid == -1) {
        /* fork() failed - typically because the system is out of
         * resources (process table full, memory limits, etc.). */
        perror("fork");
        return;
    }

    if (pid == 0) {
        /* fork() returns 0 inside the CHILD process. */
        printf("Child Process Created\n");
        printf("Child PID : %d\n", getpid());
        printf("Parent PID: %d\n", getppid());

        /* _exit() ends the child immediately without re-running the
         * rest of main()'s menu loop and without flushing stdio
         * buffers a second time (which would duplicate output that
         * the parent already buffered before fork()). */
        _exit(0);
    }

    /* fork() returns the child's PID inside the PARENT process. */
    printf("Parent Process\n");
    printf("Parent PID: %d\n", getpid());
    printf("Child PID created: %d\n", pid);

    last_child_pid = pid;
    printf("\nNote: the child has not been waited on yet.\n");
    printf("Use option 4 (Wait for Child Process) to synchronize with it.\n");
}

/* ------------------------------------------------------------------ */
/* 2. Process execution                                               */
/* ------------------------------------------------------------------ */
void execute_program(void)
{
    printf("\n--- Execute Program ---\n");

    if (last_child_pid > 0) {
        printf("Warning: child PID %d was never waited on and is no longer tracked.\n",
               last_child_pid);
    }

    printf("Enter a command to run (e.g. ls -l), or press Enter for \"ls\": ");

    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read command.\n");
        return;
    }
    discard_rest_of_line(input);
    input[strcspn(input, "\n")] = '\0'; /* strip trailing newline */

    if (input[0] == '\0') {
        strcpy(input, "ls"); /* safe default command */
    }

    /* Split the input line into argv[] tokens for execvp(). */
    char *argv[MAX_ARGS];
    int argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL && argc < MAX_ARGS - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;

    if (argc == 0) {
        fprintf(stderr, "No command entered.\n");
        return;
    }

    fflush(stdout); /* see comment in create_child_process() */

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        /*
         * fork() and exec() are used together because they solve two
         * different problems:
         *   - fork() creates a NEW process (a copy of this one).
         *   - execvp() does NOT create a process; it REPLACES the
         *     calling process's own code/data with a different
         *     program's image, reusing the same PID.
         *
         * Using fork() first means the ORIGINAL process (the parent,
         * running this menu) keeps running, while only the CHILD copy
         * is replaced by the requested command.
         */
        execvp(argv[0], argv);

        /* execvp() only returns if it failed to run the program. */
        perror("execvp");
        _exit(1);
    }

    printf("Parent PID %d launched child PID %d to run \"%s\"\n",
           getpid(), pid, argv[0]);

    last_child_pid = pid;
    printf("\nNote: the child has not been waited on yet.\n");
    printf("Use option 4 (Wait for Child Process) to synchronize with it.\n");
}

/* ------------------------------------------------------------------ */
/* 3. Process identification                                          */
/* ------------------------------------------------------------------ */
void display_process_info(void)
{
    printf("\n--- Process Information ---\n");
    /* getpid()  returns the PID of the process calling it.   */
    /* getppid() returns the PID of that process's parent.    */
    printf("Current Process ID (PID) : %d\n", getpid());
    printf("Parent Process ID (PPID) : %d\n", getppid());
}

/* ------------------------------------------------------------------ */
/* 4. Process synchronization                                         */
/* ------------------------------------------------------------------ */
void wait_for_child(void)
{
    printf("\n--- Wait for Child Process ---\n");

    if (last_child_pid <= 0) {
        printf("No child process is pending.\n");
        printf("Create one first using option 1 or option 2.\n");
        return;
    }

    printf("Parent waiting for child process...\n");

    /*
     * waitpid() is used (rather than a plain wait()) so that the
     * parent synchronizes with a SPECIFIC child by PID, rather than
     * whichever child happens to terminate first. This matters once a
     * process manages more than one child at a time.
     */
    int status;
    pid_t result = waitpid(last_child_pid, &status, 0);

    if (result == -1) {
        perror("waitpid");
        return;
    }

    printf("Child process completed.\n");

    if (WIFEXITED(status)) {
        printf("Child exited normally with status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child was terminated by signal: %d\n", WTERMSIG(status));
    }

    printf("Parent process resumed.\n");
    last_child_pid = -1; /* this child has now been fully handled */
}

/* ------------------------------------------------------------------ */
/* 5. Process termination                                             */
/* ------------------------------------------------------------------ */
void terminate_process(void)
{
    printf("\n--- Process Termination ---\n");

    if (last_child_pid > 0) {
        printf("Warning: a child process (PID %d) has not been waited on.\n",
               last_child_pid);
        printf("It will be reparented to init and cleaned up by the system.\n");
    }

    printf("Enter an exit status code (0-255): ");

    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read status code.\n");
        return;
    }
    discard_rest_of_line(input);

    int code = atoi(input);
    if (code < 0 || code > 255) {
        printf("Invalid status code, defaulting to 0.\n");
        code = 0;
    }

    printf("Terminating with exit status %d...\n", code);

    /*
     * exit() performs a controlled shutdown: it flushes stdio buffers
     * (so the printf() above is guaranteed to appear) and ends the
     * process, returning the given status to whatever waits on it.
     */
    exit(code);
}
