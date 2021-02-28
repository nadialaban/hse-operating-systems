#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    int shmid;
    int new = 1;
    char pathname[] = "task01-1.c";
    key_t key;
    char *text;
    int fsize = 0;
    int *size;
    FILE *file;

    // Открытие файла с исходным кодом программы на чтение
    file = fopen(pathname, "r");
    // Получение размера файла
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file);

    // Перемещение указателя на начало файлового потока
    rewind(file);

    // Генерация IPC-ключа
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // Создание сегмента разделяемой памяти,
    // либо получение его идентификатора, если он уже существует
    if ((shmid = shmget(key, sizeof(int) + fsize * sizeof(char),
                        0666 | IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }

    // Отображение разделяемой памяти в адресное пространство текущего процесса
    if ((size = (int *) shmat(shmid, NULL, 0)) == (int *) (-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    *size = fsize;
    text = (char *) (size + 1);

    // Запись строк файла в сегмент разделяемой памяти
    for (int i = 0; i < fsize; i++)
        text[i] = fgetc(file);

    fclose(file);

    // Удаление разделяемой памяти из адресного пространства текущего процесса
    // и завершение работы
    if (shmdt(size) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    return 0;
}
