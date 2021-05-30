#include <signal.h>

int main(void) {
    // Игнорируем сигнал SIGINT
    // Теперь <CTRL> и <C> не приводят к звершению процесса
    (void)signal(SIGINT, SIG_IGN);

    // Игнорируем сигнал SIGQUIT
    // Теперь <CTRL> и <4> не приводят к звершению процесса
    (void)signal(SIGQUIT, SIG_IGN);

    // Запускаем вечный цикл, чтобы процесс не выключился сам
    while(1);
    return 0;
}