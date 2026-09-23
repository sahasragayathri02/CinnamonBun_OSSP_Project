# Makefile - Linux Process Management System
#
# Targets:
#   make        build the "process_manager" executable
#   make clean  remove compiled object files and the executable

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
SRCDIR  = src
SRCS    = $(SRCDIR)/main.c $(SRCDIR)/process_manager.c
OBJS    = $(SRCS:.c=.o)
TARGET  = process_manager

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/process_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
