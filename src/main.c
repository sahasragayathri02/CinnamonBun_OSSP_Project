/*
 * main.c
 *
 * Terminal-based menu for the Linux Process Management System.
 * This file only handles the menu and user interaction; the actual
 * process-management logic lives in process_manager.c so each part of
 * the program has a single, clear responsibility.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "process_manager.h"

#define EOF_CHOICE (-2)
#define BAD_CHOICE (-1)

static void print_menu(void);
static int read_choice(void);
static void discard_rest_of_line(const char *buf);

int main(void)
{
    int running = 1;

    while (running) {
        print_menu();
        int choice = read_choice();

        switch (choice) {
            case 1:
                create_child_process();
                break;
            case 2:
                execute_program();
                break;
            case 3:
                display_process_info();
                break;
            case 4:
                wait_for_child();
                break;
            case 5:
                terminate_process();
                break;
            case 6:
                printf("\nExiting Linux Process Management System.\n");
                running = 0;
                break;
            case EOF_CHOICE:
                printf("\nInput stream closed. Exiting.\n");
                running = 0;
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 6.\n");
                break;
        }
    }

    return 0;
}

static void print_menu(void)
{
    printf("\n========================================\n");
    printf(" Linux Process Management System\n");
    printf("========================================\n");
    printf("1. Create Child Process\n");
    printf("2. Execute Program\n");
    printf("3. Display Process Information\n");
    printf("4. Wait for Child Process\n");
    printf("5. Terminate Process\n");
    printf("6. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

/*
 * Reads one line of input and parses it as an integer menu choice.
 * Returns EOF_CHOICE if the input stream has closed, or BAD_CHOICE if
 * the line could not be parsed as a plain number (both are handled as
 * "invalid"/exit conditions by the caller).
 */
static int read_choice(void)
{
    char line[64];

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return EOF_CHOICE;
    }

    /* If the line didn't fit in the buffer, the rest of it is still
     * waiting on stdin. Left unread, it would be picked up as the
     * *next* menu choice instead of being prompted for normally. */
    discard_rest_of_line(line);

    char *endptr;
    long value = strtol(line, &endptr, 10);

    /* Reject: no digits parsed, or a number so large it doesn't fit
     * in an int (strtol reports this via errno/clamping rather than
     * the undefined behaviour a plain sscanf("%d", ...) would have). */
    if (endptr == line || value < INT_MIN || value > INT_MAX) {
        return BAD_CHOICE;
    }

    return (int)value;
}

/*
 * If fgets() filled the buffer without reaching a newline, the input
 * line was longer than the buffer. This drains the remainder from
 * stdin so it isn't misread as a separate, unrelated menu choice on
 * the next prompt.
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
