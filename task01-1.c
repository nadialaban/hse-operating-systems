#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
  int    semid;        
  char   pathname[]="task01-1.c";
                               
  key_t  key;          
  struct sembuf mybuf; 

  // Генерируем IPC-ключ из имени файла
  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  
  // Пытаемся получить доступ по ключу к массиву семафоров,
  // Если его не существует, создаем массив из одного семафора
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
    
  // Определяем структуру
  // Счетчик устанавливаем -5, чтобы дождаться 5 запусков второй программы
  mybuf.sem_num = 0;
  mybuf.sem_op  = -5;
  mybuf.sem_flg = 0;

  // Ожидаем увеличение счетчика
  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }

  // Выводим сообщение
  printf("The condition is present\n");
    
  return 0;
}
