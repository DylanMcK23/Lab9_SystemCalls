# CPE-2600 Lab 9 - System Calls

**Name:** Dylan McKinney
**Section:** 111

## Overview
Four small C programs that exercise common POSIX system calls on Linux.

| Program | Source   | System calls used |
|---------|----------|-------------------|
| `info`  | info.c   | `clock_gettime`, `uname`, `get_nprocs`, `sysconf`, `getpagesize` |
| `pinfo` | pinfo.c  | `getpid`, `getpriority`, `sched_getscheduler` |
| `pmod`  | pmod.c   | `nice`, `nanosleep` |
| `finfo` | finfo.c  | `lstat` |

## Build
```
make        # builds info, pinfo, pmod, finfo
make clean  # removes object files and executables
```

## Usage
- `./info` - prints time of day (ns), host name, OS name/release/version,
  hardware type, number of CPUs, and total/free physical memory in bytes.
  Blank values are not printed.
- `./pinfo [pid]` - prints the priority and scheduling policy (as text) of
  the given process, or of the current process if no pid is given. Prints an
  error and exits if the pid does not exist.
- `./pmod` - raises its nice value by 10 (reducing its priority), sleeps for
  1,837,272,638 ns, prints a goodbye message, and exits.
- `./finfo <file>` - prints file type, permissions (rwx form), owner user id,
  size in bytes, and last modification time.

## Notes
- `nice(10)` is used to *reduce* priority: a higher nice value means lower
  priority. In `top`/`htop` the NI column shows 10 and PR shows 30 for the
  process while it sleeps (PR = 20 + NI).
- `man 2 nice` documents the Linux system call; `man nice` documents the
  shell command of the same name.
- `lstat` was chosen over `stat` so symbolic links are reported as links
  rather than followed; `fstat` needs an already-open file descriptor.
- `pinfo.c` defines `_GNU_SOURCE` before any includes so that `SCHED_BATCH`
  and `SCHED_IDLE` (Linux-specific) compile.