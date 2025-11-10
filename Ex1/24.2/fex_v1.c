#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    write(1, argv[1], 1);
    write(1, "\n", 1);
    if (argc == 4)
        fork();
    printf("%d (%d)\n", getpid(), getppid());
    if (argc < 3 || *argv[1] == *argv[2])
        exit(0);
    ++*argv[1];
    execl(argv[0], argv[0], argv[1], argv[2], argv[3], (char *)NULL);
    write(1, "Oops...\n", 8);
}