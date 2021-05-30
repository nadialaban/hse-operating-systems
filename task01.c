#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    // Удаляем старую папку
    const char* folder = "/files";
    struct stat sb;

    if (stat(folder, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        system("rm -r files");
    }

    // Создаем папку и заходим в нее
    system("mkdir files");
    chdir("./files");

    // Файловый декриптор
    int fd;
    // Путь к файлу
    char *filename = "1.txt";

    // Пробуем создать файл
    if ((fd = open(filename, O_CREAT | O_WRONLY)) == -1) {
        printf("Can\'t create file!\n");
        exit(-1);
    }

    // Закрыывем поток и проверяем, что все правильно
    if (close(fd) < 0) {
        printf("Can\'t close file!\n");
        exit(-1);
    }

    // Названия файлов
    char *file = malloc(1000);
    char *link = malloc(1000);
    int depth = 1;

    // Создаем мягкие связи
    while (1) {
        sprintf(file, "%d.txt", depth);
        sprintf(link, "%d.txt", depth + 1);

        // Проверяем, создалась ли мягкая связь
        if (symlink(file, link) == 0) {
            // Пробуем открыть связь
            if ((fd = open(link, O_RDONLY)) < 0) {
                printf("Can\'t open link %s\n", link);
                printf("Recursion depth: %d\n", depth - 1);
                exit(-1);
            }
            // Пробуем закрыть свзь
            if (close(fd) < 0) {
                printf("Can\'t close link %s!\n", link);
                exit(-1);
            }
        } else {
            printf("Can\'t create link for file %s\n", file);
            exit(-1);
        }
        depth++;
    }

    return 0;
}