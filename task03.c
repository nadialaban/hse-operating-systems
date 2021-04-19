#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Изначально значение семафора равно 1
 * В цикле в родительском процессе после записи сообщения
 * счетчик уменьшается с помощью операции D,
 * тем самым блокируя семафор и
 * давая дочернему процессу прочесть сообщение.
 * После прочтения и записи сообщения счетчик увеличивается с помощью операции A,
 * тем самым снимая блокировку с семафора
 * и позволяя родительскому процессу прочесть сообщение.
 */

int D(int sem, struct sembuf *buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(sem, buf, 1);
}

int A(int sem, struct sembuf *buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(sem, buf, 1);
}

int main() {
    // Количество итераций программы
    int N, i;
    printf("Enter count of iterations (>2):\n> ");
    scanf("%d", &N);
    if (N < 2) {
        printf("incorrect! N = 2\n");
        N = 2;
    }

    // Путь к этому файлу
    char pathname[] = "task03.c";

    // IPC ключ
    key_t key;

    // ID семафора и буфер
    int semid;
    struct sembuf mybuf;

    // Дескриптор для связи
    int fd[2], result;

    // Переменные для записи считанного сообщения
    int size;
    char resstring[16];

    // Создаем pipe и проверяем, что все хорошо
    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    // Генерируем IPC-ключ из имени файла
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // Пытаемся получить доступ по ключу к массиву семафоров
    if ((semid = semget(key, 1, 0666)) < 0) {
        // Если его нет, пытаемся создать
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t create semaphore set\n");
            exit(-1);
        }

        // Выводим сообщение
        printf("Success\n");
    }

    // Создаем дочерний процесс
    result = fork();

    for (i = 0; i < N; ++i) {
        // Проверяем, что процесс создан
        if (result < 0) {
            printf("Can\'t fork child[%d]\n", i);
            exit(-1);
        } else if (result > 0) {

            /* Parent process */
            // Выводим текущее состояние
            printf("parent[%d]: start\n", i);

            // Записываем сообщение
            size = write(fd[1], "Hello, world![p]", 16);

            // Проверяем, что сообщение записалось полностью
            if (size != 16) {
                printf("parent[%d]: can\'t write all string to pipe\n", i);
                exit(-1);
            }
            printf("parent[%d]: wrote message to child\n", i);

            // Уменьшаем счетчик
            // Таким образом блокируем процесс родителя, пока работает ребенок
            // Он проделжит работу, когда счетчик увеличится
            D(semid, &mybuf);

            // Считываем сообщение
            size = read(fd[0], resstring, 16);

            // Проверяем, что все считалось
            if (size < 0) {
                printf("parent[%d]: can\'t read string from pipe\n", i);
                exit(-1);
            }

            // Выводим прочитанное сообщение
            printf("parent[%d]: read message from child\n> %s\n", i, resstring);

            // Выводим текущее состояние
            printf("parent[%d]: end\n", i);
        } else {

            /* Child process */
            // Выводим текущее состояние
            printf("child[%d]: start\n", i);

            // Считываем сообщение
            size = read(fd[0], resstring, 16);

            // Проверяем, что все считалось
            if (size < 0) {
                printf("child[%d]: can\'t read string from pipe\n", i);
                exit(-1);
            }

            // Выводим прочитанное сообщение
            printf("child[%d]: read message from parent\n> %s\n", i, resstring);

            // Записываем сообщение
            size = write(fd[1], "Hello, world![с]", 16);

            // Проверяем, что сообщение записалось полностью
            if (size != 16) {
                printf("child[%d]: can\'t write all string to pipe\n", i);
                exit(-1);
            }
            printf("child[%d]: wrote message to parent\n", i);

            // Увеличиваем счетчик
            // Таким образом снимаем блокировку с процесса родителя
            A(semid, &mybuf);

            // Выводим текущее состояние
            printf("child[%d]: end\n", i);
        }
    }

    // Закрываем pipe
    close(fd[0]);
    close(fd[1]);

    return 0;
}
