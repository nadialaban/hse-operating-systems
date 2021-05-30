#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
    // Обрабатываем в зависимости от сигнала
    // SIGINT - nsig = 2
    // SIGQUIT - nsig = 3
    switch (nsig) {
        case 2:
            printf("You pressed CTRL + C\n");
            printf("Received signal: SIGINT [%d]\n", nsig);
            break;
        case 3:
            printf("You pressed CTRL + 4\n");
            printf("Received signal: SIGQUIT [%d]\n", nsig);
            break;
    }
}

int main(void) {
    // Обрабатываем сигнал SIGINT
    // Теперь <CTRL> и <C> не приводят к звершению процесса
    (void)signal(SIGINT, my_handler);

    // Обрабатываем сигнал SIGQUIT
    // Теперь <CTRL> и <4> не приводят к звершению процесса
    (void)signal(SIGQUIT, my_handler);

    // Запускаем вечный цикл, чтобы процесс не выключился сам
    while(1);
    return 0;
}