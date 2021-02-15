#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Дескрипторы для каждой связи
    int fd_parentToChild[2],
        fd_childToParent[2],
        result;

    // Переменные для записи считанного сообщения
    int size;
    char resstring1[19];
    char resstring2[18];

    // Создаем трубы и проверяем, что все хорошо
    if (pipe(fd_parentToChild) < 0) {
        printf("Can\'t open parent_to_child pipe\n");
        exit(-1);
    }

    if (pipe(fd_childToParent) < 0) {
        printf("Can\'t open parent_to_child pipe\n");
        exit(-1);
    }

    // Создаем дочерний процесс
    result = fork();

    // Проверяем, что процесс создан
    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {

        /* Parent process */

        // Закрываем трубу родитель-ребенок на чтение
        if (close(fd_parentToChild[0]) < 0) {
            printf("parent: Can\'t close reading side of pipe\n");
            exit(-1);
        }

        // Записываем сообщение от родителя
        size = write(fd_parentToChild[1], "Hello from parent!", 19);

        // Проверяем, что сообщение записалось полностью
        if (size != 19) {
            printf("parent: can\'t write all string to pipe\n");
            exit(-1);
        }

        // Закрываем трубу родитель-ребенок на запись
        if (close(fd_parentToChild[1]) < 0) {
            printf("parent: Can\'t close writing side of pipe\n");
            exit(-1);
        }
        
        // Выводим текущее состояние
        printf("parent: wrote message to child\n");
        printf("parent: sleep\n");

    } else {

        /* Child process */

        // Закрываем трубу родитель-ребенок на запись
        if (close(fd_parentToChild[1]) < 0) {
            printf("child: can\'t close writing side of pipe\n");
            exit(-1);
        }

        // Считываем текст из трубы родитель-ребенок
        size = read(fd_parentToChild[0], resstring1, 19);

        // Проверяем, что все считалось
        if (size < 0) {
            printf("child: can\'t read string from pipe\n");
            exit(-1);
        }

        // Выводим прочитанное сообщение
        printf("child: read message from parent\n");
        printf("> %s\n", resstring1);

        // Закрываем трубу родитель-ребенок на чтение
        if (close(fd_parentToChild[0]) < 0) {
            printf("child: can\'t close reading side of pipe\n");
            exit(-1);
        }

        // Закрываем трубу ребенок-родитель на чтение
        if (close(fd_childToParent[0]) < 0) {
            printf("child: can\'t close reading side of pipe\n"); exit(-1);
        }

        // Записываем сообщение от ребенка
        size = write(fd_childToParent[1], "Hello from child!", 18);

        // Проверяем, что все записалось
        if (size != 18) {
            printf("child: can\'t write all string to pipe\n");
            exit(-1);
        }

        // Закрываем трубу ребенок-родитель на запись
        if (close(fd_childToParent[1]) < 0) {
            printf("child: can\'t close writing side of pipe\n");
            exit(-1);
        }

        // Выводим текущее состояние
        printf("child: wrote message to parent\n");
        printf("child: exit\n");
    }

    // Задержка, чтобы все корректно работало
    sleep(2);

    if (result > 0) {

        /* Parent process */

        // Закрываем трубу ребенок-родитель на запись
        if (close(fd_childToParent[1]) < 0) {
            printf("parent: can\'t close writing side of pipe\n");
            exit(-1);
        }

        // Считываем сообщение от ребенка
        size = read(fd_childToParent[0], resstring2, 18);

        // Проверяем, что все считалось
        if (size < 0) {
            printf("parent: can\'t read string from pipe\n");
            exit(-1);
        }

        // Выводим сообщение от ребенка
        printf("parent: read message from child\n");
        printf("> %s\n", resstring2);

        // Закрываем трубу ребенок-родитель на чтение
        if (close(fd_childToParent[0]) < 0) {
            printf("parent: can\'t close reading side of pipe\n");
            exit(-1);
        }

        printf("parent: exit\n");
    }

    return 0;
}
