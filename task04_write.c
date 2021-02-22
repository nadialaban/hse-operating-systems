#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fd;
    
    // Размер записанного сообщения
    size_t size;
    // Переменная для сообщения
    char str[14];

    (void)umask(0);

    // Создаем FIFO
    if (mknod("a.fifo", S_IFIFO | 0666, 0) < 0) {
        printf("Can\'t create FIFO\n");
        exit(-1);
    }

    // Открываем FIFO на запись
    if ((fd = open("a.fifo", O_WRONLY)) < 0) {
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }

    // Записываем сообщение
    size = write(fd, "Hello, world!", 14);

    // Проверяем, что все записалось
    if (size != 14) {
        printf("Can\'t write all string to FIFO\n");
        exit(-1);
    }

    // Закрываем FIFO
    if (close(fd) < 0) {
        printf("Can\'t close FIFO\n");
        exit(-1);
    }

    return 0;
}
