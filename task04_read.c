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

    // Открываем FIFO на чтение
    if ((fd = open("a.fifo", O_RDONLY)) < 0) {
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }

    // Читаем сообщение
    size = read(fd, str, 14);

    // Проверяем, что все прочиталось
    if (size < 0) {
        printf("Can\'t read string from FIFO\n");
        exit(-1);
    }

    // Выводим ответ
    printf("Message:\t%s\n", str);

    // Закрываем FIFO
    if (close(fd) < 0) {
        printf("Can\'t close FIFO\n");
        exit(-1);
    }

    return 0;
}