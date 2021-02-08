#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {

    printf("\nARGUMENTS:\n\n");
    for (int i = 0; i < argc; ++i) {
        printf("%s\t", argv[i]);
    }
    printf("\n----------------------------\n");
    printf("\nENVIRONMENT PARAMS:\n\n");
    int i = 0;
    while (envp[i] != NULL) {
        printf("%s\n", envp[i++]);
    }
    return 0;
}
