#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>

int intFromStr(char *s) {
    int res = 0;
    for (int i = 0; i < strlen(s); ++i) {
        res = res * 10 + s[i] - '0';
    }
    return res;
}

int main(int argc, char** argv) {
    //pr1 >> res || pr2 | pr3
    if (argc != 4) {
        printf("need 4 arguments\n");
        return -1;
    }
    char *pr1Name = argv[1];
    char *resName = "res";
    char *pr2Name = argv[2];
    char *pr3Name = argv[3];
    int numSec = intFromStr(argv[4]);

    int fd1[2];
    if (pipe(fd1) == -1) {
        fprintf(stderr, "Can't create pipe\n");
        exit(EXIT_FAILURE);
    }
    dup2(fd1[1], STDOUT_FILENO);
    if (execve(argv[2], argv + 2, (char**) NULL)) {
        error(1, errno, "parent");
    }
    FILE* file = fopen(resName, "a");
    if (file == NULL) {
        printf("file has not opened\n");
        return -1;
    }
    char buff[1000];
    ssize_t cnt;
    while ((cnt = read(fd1[0], buff, 1000)) != 0) {
        if (!fwrite(buff, (size_t)cnt, 1, file)) {
            printf("can't write to file\n");
            return -1;
        }
    }


    printf("Hello, World!\n");
    return 0;
}