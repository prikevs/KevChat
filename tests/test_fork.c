#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int g = 6;
int *p = &g;

int main()
{
    pid_t pid;

    if ((pid = vfork()) < 0) {
        printf("fork error"); 
    }
    else if (pid == 0) {
        (*p)++; 
    }
    else {
        sleep(2); 
    }

    printf("pid = %d, g = %d, addr = %ld\n", getpid(), g, p);
    exit(0);
}
