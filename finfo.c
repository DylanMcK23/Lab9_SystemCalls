/*
 * Name:       Dylan McKinney
 * Assignment: CPE-2600 Lab 9 - System Calls
 * Section:    111
 *
 * finfo.c
 *
 * Part 3: File Management System Calls.
 * Usage: ./finfo <filename>
 * Displays type, permissions, owner, size and last modification time
 * of a file using lstat.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Return a readable description of the file type from st_mode. */
static const char* file_type_string(mode_t mode)
{
    if (S_ISREG(mode))  return "Regular file";
    if (S_ISDIR(mode))  return "Directory";
    if (S_ISLNK(mode))  return "Symbolic link";
    if (S_ISCHR(mode))  return "Character device";
    if (S_ISBLK(mode))  return "Block device";
    if (S_ISFIFO(mode)) return "FIFO (named pipe)";
    if (S_ISSOCK(mode)) return "Socket";
    return "Unknown";
}

/* Build an "rwxrwxrwx" style string from the permission bits. */
static void permission_string(mode_t mode, char out[10])
{
    out[0] = (mode & S_IRUSR) ? 'r' : '-';
    out[1] = (mode & S_IWUSR) ? 'w' : '-';
    out[2] = (mode & S_IXUSR) ? 'x' : '-';
    out[3] = (mode & S_IRGRP) ? 'r' : '-';
    out[4] = (mode & S_IWGRP) ? 'w' : '-';
    out[5] = (mode & S_IXGRP) ? 'x' : '-';
    out[6] = (mode & S_IROTH) ? 'r' : '-';
    out[7] = (mode & S_IWOTH) ? 'w' : '-';
    out[8] = (mode & S_IXOTH) ? 'x' : '-';
    out[9] = '\0';
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    /* lstat is used (rather than stat) so that if the argument is a
     * symbolic link we report on the link itself instead of silently
     * following it; that lets us display the "symbolic link" type.
     * (fstat would need an already-open file descriptor.) */
    struct stat info;
    if (lstat(argv[1], &info) == -1)
    {
        fprintf(stderr, "Error: cannot stat '%s': %s\n",
                argv[1], strerror(errno));
        return 1;
    }

    char perms[10];
    permission_string(info.st_mode, perms);

    /* Format the modification time as a readable local date/time. */
    char timebuf[64];
    struct tm* local = localtime(&info.st_mtime);
    if (local == NULL ||
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", local) == 0)
    {
        fprintf(stderr, "Error: cannot format modification time\n");
        return 1;
    }

    printf("File:               %s\n", argv[1]);
    printf("Type:               %s\n", file_type_string(info.st_mode));
    printf("Permissions:        %s (%04o)\n", perms,
           (unsigned int)(info.st_mode & 07777));
    printf("Owner (user id):    %u\n", (unsigned int)info.st_uid);
    printf("Size (bytes):       %lld\n", (long long)info.st_size);
    printf("Last modified:      %s\n", timebuf);

    return 0;
}