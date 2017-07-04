#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int min(int a, int b) {
    return a < b ? a : b;
}

void replaceStr(char *str) {
    int curIndex = 0;
    for (int i = 0; i < strlen(str); ) {
        if (!isDigit(str[i])) {
            str[curIndex++] = str[i++];
        } else {
            int j = i;
            int count = 0;
            int sum = 0;
            while (j < strlen(str) && isDigit(str[j])) {
                count++;
                sum += str[j] - '0';
                j++;
            }
            i = j;
            for (j = 0; j < min(count, sum); ++j) {
                str[curIndex++] = '*';
            }
        }
    }
    str[curIndex] = 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "no argument\n");
        return -1;
    }
    char* fileName = argv[1];
    FILE* openFile;
    if ((openFile = fopen(fileName, "r")) == NULL) {
        printf("main: %s:%s\n", fileName, strerror(errno));
    } else {
        size_t n = 0;
        char* strBuff = NULL;
        while (getline(&strBuff, &n, openFile) != EOF) {
            replaceStr(strBuff);
            printf("%s", strBuff);
        }
        free(strBuff);
    }
    fclose(openFile);

    return 0;
}