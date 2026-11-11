/*
 * Name:       Dylan McKinney
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    111
 *
 * info.c
 *
 * Part 1: Information Maintenance System Calls.
 * Prints general information about the running system using
 * clock_gettime, uname, get_nprocs, sysconf and getpagesize.
 * Any value that is blank (NULL / empty string) is NOT printed.
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

#define NSEC_PER_SEC 1000000000LL

/*
 * Prints "label value" only when the string value is non-NULL
 * and not empty (first char is not the NULL terminator).
 */
static void print_if_set(const char* label, const char* value)
{
    if (value != NULL && value[0] != '\0')
    {
        printf("%s%s\n", label, value);
    }
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    /* ---- Current time of day, in nanoseconds ---- */
    /* clock_gettime fills a timespec (seconds + nanoseconds). CLOCK_REALTIME
     * is the wall-clock time since the Unix epoch. Combine both fields into
     * one nanosecond count. */
    struct timespec now;
    if (clock_gettime(CLOCK_REALTIME, &now) == 0)
    {
        long long ns = (long long)now.tv_sec * NSEC_PER_SEC + now.tv_nsec;
        printf("Current time (ns since epoch): %lld\n", ns);
    }
    else
    {
        perror("clock_gettime");
    }

    /* ---- System names via uname ---- */
    /* uname fills a struct utsname with the OS name, network node name,
     * release, version and hardware type (machine). */
    struct utsname sysinfo;
    if (uname(&sysinfo) == 0)
    {
        print_if_set("Network name:      ", sysinfo.nodename);
        print_if_set("Operating system:  ", sysinfo.sysname);
        print_if_set("OS release:        ", sysinfo.release);
        print_if_set("OS version:        ", sysinfo.version);
        print_if_set("Hardware type:     ", sysinfo.machine);
    }
    else
    {
        perror("uname");
    }

    /* ---- Number of CPUs ---- */
    /* get_nprocs returns the number of processors currently online. */
    printf("Number of CPUs:    %d\n", get_nprocs());

    /* ---- Memory in bytes ---- */
    /* sysconf reports memory as a count of pages, so multiply the page
     * count by the page size (getpagesize) to get bytes.
     * _SC_PHYS_PAGES   = total physical pages
     * _SC_AVPHYS_PAGES = currently free (available) physical pages */
    long page_size = getpagesize();
    long phys_pages = sysconf(_SC_PHYS_PAGES);
    long free_pages = sysconf(_SC_AVPHYS_PAGES);

    if (phys_pages >= 0)
    {
        printf("Total memory (bytes): %lld\n",
               (long long)phys_pages * page_size);
    }
    else
    {
        perror("sysconf(_SC_PHYS_PAGES)");
    }

    if (free_pages >= 0)
    {
        printf("Free memory (bytes):  %lld\n",
               (long long)free_pages * page_size);
    }
    else
    {
        perror("sysconf(_SC_AVPHYS_PAGES)");
    }

    return 0;
}