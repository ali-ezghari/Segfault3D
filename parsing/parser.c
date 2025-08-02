#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>


void exit_error(int stauts, char *message)
{
    if (stauts == 2)
        write (2, "Error: ", 7);
    write(stauts, message, strlen(message));
    exit(stauts);
}

int is_valid_name_file(char *argv)
{
    int len;

    len = strlen(argv);
    if (strstr(argv + (len - 4), ".cub"))
        return (1);
    return (0);
}

int init_data(int fd)
{
    
}

int parser(int argc, char *argv)
{
    int fd;

    if (argc == 2)
    {
        if (!is_valid_name_file(argv))
            exit_error(2, "Invalid name of file.\n");

        fd = open(argv, O_RDONLY);
        if (fd != -1)
        {
            // printf("--- FILE [%d] OPEN SECSAPHELY ---\n", fd);
            init_data(fd);
        }
        else if (fd == -1)
        {
            exit_error(errno, "Opening file.\n");
        }
    }
    else
    {
        exit_error(1, "Invalid number of arguments.\n");
    }
    return (0);
}

int main(int argc, char *argv[])
{
    parser(argc, argv[1]);
}