/*
 * Name:       Dylan McKinney
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    111 
 *
 * pinfo.c
 *
 * Part 2: Process Control System Calls (retrieving process information).
 * Usage: ./pinfo [pid]
 * Prints the priority and scheduling policy (as text) of the given process.
 * With no argument, reports on the current process.
 */

/* Must come BEFORE any header so SCHED_BATCH / SCHED_IDLE are defined */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>

/* Convert a scheduling policy number into a readable string. */
static const char* policy_to_string(int policy)
{
    switch (policy)
    {
        case SCHED_OTHER: return "SCHED_OTHER";
        case SCHED_FIFO:  return "SCHED_FIFO";
        case SCHED_RR:    return "SCHED_RR";
        case SCHED_BATCH: return "SCHED_BATCH";
        case SCHED_IDLE:  return "SCHED_IDLE";
        default:          return "Unknown";
    }
}

int main(int argc, char* argv[])
{
    pid_t pid;

    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s [pid]\n", argv[0]);
        return 1;
    }

    if (argc == 2)
    {
        /* Parse the pid with strtol so bad input can be detected. */
        char* end = NULL;
        errno = 0;
        long value = strtol(argv[1], &end, 10);
        if (errno != 0 || end == argv[1] || *end != '\0' ||
            value <= 0 || value > INT_MAX)
        {
            fprintf(stderr, "Error: '%s' is not a valid process id\n",
                    argv[1]);
            return 1;
        }
        pid = (pid_t)value;
    }
    else
    {
        /* No pid given: use the current process. */
        pid = getpid();
    }

    /* getpriority can legitimately return -1, so clear errno first and
     * check it afterwards to detect a real failure (e.g. ESRCH). */
    errno = 0;
    int priority = getpriority(PRIO_PROCESS, (id_t)pid);
    if (priority == -1 && errno != 0)
    {
        fprintf(stderr, "Error: cannot get priority of process %d: %s\n",
                (int)pid, strerror(errno));
        return 1;
    }

    /* sched_getscheduler returns the policy, or -1 on error. */
    int policy = sched_getscheduler(pid);
    if (policy == -1)
    {
        fprintf(stderr, "Error: cannot get scheduler of process %d: %s\n",
                (int)pid, strerror(errno));
        return 1;
    }

    printf("Process ID:        %d\n", (int)pid);
    printf("Process priority:  %d\n", priority);
    printf("Scheduling method: %s\n", policy_to_string(policy));

    return 0;
}