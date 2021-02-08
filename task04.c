#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t thisProcessID = getpid();
    pid_t parentProcessID = getppid();
    pid_t forkedProcessID = fork();

    printf("PID = %d\n", thisProcessID);
    printf("PPID = %d\n", parentProcessID);

    if (forkedProcessID < 0) {
        printf("Some problems with forking process... (%d)\n\n", forkedProcessID);
        return -1;
    }
    else if (forkedProcessID == 0) {
        printf("This is from child process. (%d)\n", forkedProcessID);
        printf("Let's start task03 \n\n");
        execl("./task03.out", "hello,", "world!", NULL);
    }
    else
        printf("This is from parent process. (%d)\n\n", forkedProcessID);

    return 0;
}
