/*
 * Name:       Dylan McKinney
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    111
 *
 * pmod.c
 *
 * Part 2: Process Control System Calls (altering a process).
 * Lowers its own priority by 10, sleeps for 1,837,272,638 ns,
 * then prints a goodbye message and exits.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    /* nice() ADDS its argument to the "nice value". A higher nice value
     * means a LOWER priority, so +10 reduces our priority by 10.
     * nice() can return -1 on success, so clear errno and check it. */
    errno = 0;
    int result = nice(10);
    if (result == -1 && errno != 0)
    {
        fprintf(stderr, "Error: nice failed: %s\n", strerror(errno));
        return 1;
    }
    printf("New nice value: %d\n", result);

    /* 1,837,272,638 ns = 1 second + 837,272,638 ns.
     * tv_nsec must be less than 1,000,000,000. */
    struct timespec request = { .tv_sec = 1, .tv_nsec = 837272638L };
    struct timespec remaining;

    /* If a signal interrupts the sleep, nanosleep returns -1 with EINTR
     * and stores the unslept time in 'remaining'; resume with that. */
    while (nanosleep(&request, &remaining) == -1)
    {
        if (errno != EINTR)
        {
            fprintf(stderr, "Error: nanosleep failed: %s\n",
                    strerror(errno));
            return 1;
        }
        request = remaining;
    }

    printf("Goodbye!\n");
    return 0;
}