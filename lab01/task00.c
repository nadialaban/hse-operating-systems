#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Hello, %u from %u!\n", getuid(), getgid());
    return 0;
}
