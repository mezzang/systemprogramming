#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define BUFFERSIZE 16384
#define COPYMODE 0644

void oops(char *, char *);

int main(int ac, char *av[]){
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];

    clock_t start, end;
    double cpu_time_used;

    if (ac != 3){
        fprintf(stderr, "usage: %s ..\n", *av);
        exit(1);
    }

    start = clock();

    if ((in_fd = open(av[1], O_RDONLY)) == -1 )
        oops("Cannot open ", av[1]);
    if ((out_fd = creat(av[2], COPYMODE)) == -1 )
        oops("Cannot creat", av[2]);

    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
        if (write(out_fd, buf, n_chars) != n_chars )
            oops("Write error to ", av[2]);
    if (n_chars == -1)
        oops("Read Error from ", av[1]);

    if (close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");

    end = clock();

    cpu_time_used = ((double) end - start) / CLOCKS_PER_SEC;
    printf("CPU Time: %f\n", cpu_time_used);

    return 0;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}
