#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    int size = 14;
    char text[size];
    char *filename = "myfile";

    (void)umask(0);

    // Открываю файл, который создал запуск программы из примера
    if ((fd = open(filename, O_RDONLY)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    // Считываю в переменную text
    if (read(fd, text, size) != size) {
        printf("Can\'t read file\n");
        exit(-1);
    }

    // Вывожу
    printf("%d bytes from file %s:\n%s\n", size, filename, text);

    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }

    return 0;
}
