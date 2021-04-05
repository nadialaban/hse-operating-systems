#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;  

/**
  Below follows the text of the function
  that will be associated with the 2nd thread.

  Parameter dummy is used only for type compatibility.
  For the same reason, the function returns void *.
**/
void *mythread(void *dummy) {
    pthread_t mythid;
    mythid = pthread_self();

    a = a + 1;
    printf("Thread %u, Calculation result = %d\n", mythid, a);
    return NULL;
}

/**
  The main() function is also an associated function of the main thread.
**/
int main() {
    pthread_t thid, mythid;
    pthread_t thid2;
    int result;

    // Создаем нить исполнения
    result = pthread_create(&thid, (pthread_attr_t *) NULL, mythread, NULL);

    // Проверяем, все ли хорошо
    if (result != 0) {
        printf("Error on thread create, return value = %d\n", result);
        exit(-1);
    }

    // Выводим сообщение
    printf("Thread 1 created, thid = %u\n", thid);

    // Создаем новую нить исполнения
    result = pthread_create(&thid2, (pthread_attr_t *) NULL, mythread, NULL);

    // Проверяем, все ли хорошо
    if (result != 0) {
        printf("Error on thread create, return value = %d\n", result);
        exit(-1);
    }

    // Выводим сообщение
    printf("Thread 2 created, thid = %u\n", thid2);

    // Запрос идентефикатора главного thread'а
    mythid = pthread_self();
    a = a + 1;
    printf("Thread %u, Calculation result = %d\n", mythid, a);

    // Ожидаем завершения порожденных thread'ов
    pthread_join(thid, (void **) NULL);
    pthread_join(thid2, (void **) NULL);

    return 0;
}
