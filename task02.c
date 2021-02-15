#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fd_parentToChild[2],
        fd_childToParent[2],
        result;

    int size;
    char resstring1[19];
    char resstring2[18];
//    char *resstring1 = NULL;

    if (pipe(fd_parentToChild) < 0) {
        printf("Can\'t open parent_to_child pipe\n");
        exit(-1);
    }

    if (pipe(fd_childToParent) < 0) {
        printf("Can\'t open parent_to_child pipe\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {

        /* Parent process */

        if (close(fd_parentToChild[0]) < 0) {
            printf("parent: Can\'t close reading side of pipe\n");
            exit(-1);
        }

        size = write(fd_parentToChild[1], "Hello from parent!", 19);

        if (size != 19) {
            printf("parent: can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(fd_parentToChild[1]) < 0) {
            printf("parent: Can\'t close writing side of pipe\n");
            exit(-1);
        }

        printf("parent: wrote message to child\n");
        printf("parent: sleep\n");

    } else {

        /* Child process */

        if (close(fd_parentToChild[1]) < 0) {
            printf("child: can\'t close writing side of pipe\n");
            exit(-1);
        }

        size = read(fd_parentToChild[0], resstring1, 19);

        if (size < 0) {
            printf("child: can\'t read string from pipe\n");
            exit(-1);
        }

        printf("child: read message from parent\n");
        printf("> %s\n", resstring1);

        if (close(fd_parentToChild[0]) < 0) {
            printf("child: can\'t close reading side of pipe\n");
            exit(-1);
        }

        if (close(fd_childToParent[0]) < 0) {
            printf("child: can\'t close reading side of pipe\n"); exit(-1);
        }

        size = write(fd_childToParent[1], "Hello from child!", 18);

        if (size != 18) {
            printf("child: can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(fd_childToParent[1]) < 0) {
            printf("child: can\'t close writing side of pipe\n");
            exit(-1);
        }

        printf("child: wrote message to parent\n");
        printf("child: exit\n");
    }

    sleep(2);

    if (result > 0) {

        /* Parent process */

        if (close(fd_childToParent[1]) < 0) {
            printf("parent: can\'t close writing side of pipe\n");
            exit(-1);
        }

        size = read(fd_childToParent[0], resstring2, 18);

        if (size < 0) {
            printf("parent: can\'t read string from pipe\n");
            exit(-1);
        }

        printf("parent: read message from child\n");
        printf("> %s\n", resstring2);

        if (close(fd_childToParent[0]) < 0) {
            printf("parent: can\'t close reading side of pipe\n");
            exit(-1);
        }

        printf("parent: exit\n");
    }

    return 0;
}
