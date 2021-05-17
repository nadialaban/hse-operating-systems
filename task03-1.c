#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    // IPC descriptor для очереди сообщений
    int msqid;

    // IPC-ключ и название фвйла для генерации
    key_t key;
    char pathname[] = "task03-1.c";

    // Длина полезной части сообщения
    int len, maxlen;

    // Структура сообщения
    struct msgbuf {
        long mtype;
        struct {
            pid_t pid;
            float finfo;
        } info;
    } mybuf;

    // Структура сообщения сервера
    struct servmsgbuf {
        long mtype;
        float finfo;
    } myservbuf;

    // Генерируем IPC-ключ
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    // Пытаемся получить доступ к очереди сообщений
    // или создать ее, если она не существует
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    // В бесконечном цикле читаем сообщения
    // В порядке FIFO (First In - First Out, как в обычной очереди)
    while (1) {
        // Задаем максимально возможную длину полезной чати сообщения
        maxlen = sizeof(mybuf.info);

        // Пробуем прочесть сообщение из очереди
        if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 1, 0)) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }

        printf("Request from client %d\n", mybuf.info.pid);

        // Заполняем сообщение данными
        myservbuf.mtype = mybuf.info.pid;

        // Считаем результат
        myservbuf.finfo = mybuf.info.finfo * mybuf.info.finfo;

        // Определяем длину полезной части
        len = sizeof(myservbuf.finfo);

        // Отправляем сообщение в очередь
        if (msgsnd(msqid, (struct servmsggbuf *)&myservbuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
            exit(-1);
        }

        printf("\nResponded\n");
    }

    return 0;
}