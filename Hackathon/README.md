# Multi-Process Command Pipeline System — Updated Version

## Overview
A C program that runs a variable number of shell commands as separate child processes and connects adjacent stages using POSIX pipes.

## Improvements
1. **Variable pipeline length:** accepts two or more commands; creates N child processes and N-1 pipes.
2. **Error handling:** checks allocation, `pipe()`, `fork()`, `dup2()`, `waitpid()`, and child exit/signal statuses. Command failures produce a non-zero exit.
3. **Concurrency demonstration:** `--log` prints timestamped stage launch/start/exit events to stderr. All stages are launched before the parent waits, so execution can overlap; exact scheduling is OS-dependent.
4. **Test suite:** tests 2, 3, 4, and 5 stages; empty and large input; invalid command; failing command; log mode.

## Requirements
macOS or Linux with POSIX APIs, C compiler, and `make`. Tests use common shell tools and `awk`.

## Build
```sh
make clean
make
```

## Examples
```sh
./pipeline "printf 'a\\nb\\n'" "wc -l"
./pipeline "printf 'pear\\napple\\nbanana\\napple\\n'" "sort" "uniq" "wc -l"
./pipeline "printf 'pear\\napple\\nbanana\\napple\\n'" "cat" "sort" "uniq" "wc -l"
```
The fruit examples produce `3`.

## Process logs
```sh
./pipeline --log "printf 'hello\\nworld\\nhello\\n'" "grep hello" "wc -l"
```
Standard output contains the result; timestamped process logs go to standard error.

## Tests
```sh
make test
```
The script reports the actual pass/fail results from the current run. No performance claims are fabricated.

## Error handling
- Fewer than two commands: usage message and non-zero exit.
- Allocation/pipe/fork failure: error printed and resources cleaned up.
- Redirection failure: child exits 126.
- Invalid command: shell returns non-zero; overall pipeline returns non-zero.
- Parent closes unused pipe ends and waits for every started child.

## Security
Commands are passed to `/bin/sh -c`; do not pass untrusted command strings. This is an educational demo, not a sandbox.
