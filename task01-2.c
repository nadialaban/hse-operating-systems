#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void) {
    // IPC descriptor для очереди сообщений
    int msqid;

    // IPC-ключ и название фвйла для генерации
    key_t key;
    char pathname[] = "task01-1.c";

    // Структура сообщения
    struct mymsgbuf {
        long mtype;
        struct {
            short sinfo;
            float finfo;
        } info;
    } mybuf;

    // Длина и максимальная длина полезной части сообщения
    int len, maxlen;
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

    /* Чтение сообщений */

    // В бесконечном цикле читаем сообщения
    // В порядке FIFO (First In - First Out, как в обычной очереди)
    while (1) {
        // Задаем максимально возможную длину полезной чати сообщения
        maxlen = sizeof(mybuf.info);

        // Пробуем прочесть сообщение из очереди
        if ((len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        // Если получили сообщение с типом LAST_MESSAGE,
        // завершаем считывание
        if (mybuf.mtype == LAST_MESSAGE) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(0);
        }

        // Выводм сообщение
        printf("message type = %ld, info = [%d ; %f]\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
    }

    return 0;
}
