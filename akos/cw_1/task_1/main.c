#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int intFromStr(char *s) {
    int res = 0;
    for (int i = 0; i < strlen(s); ++i) {
        res = res * 10 + s[i] - '0';
    }
    return res;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int getBitFromByte(unsigned char a, size_t c) {
    return (a & (unsigned char) (1 << (sizeof(unsigned char) * 8 - c - 1))) != 0;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Need 4 arguments\n");
        return -1;
    }
    int p1 = intFromStr(argv[1]);
    int p2 = intFromStr(argv[2]);
    int n = intFromStr(argv[3]);
    char* fileName = argv[4];
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "file not open\n");
        return -1;
    }
    size_t sizeUCharInBits = sizeof(unsigned char) * 8;
    size_t needByte = ((n + max(p1, p2) + sizeUCharInBits - 1) / (sizeUCharInBits));
    unsigned char* ptr = malloc(needByte * sizeUCharInBits / 8);
    size_t countReadBytes = fread(ptr, 1, needByte, file);
    if (countReadBytes != needByte) {
        fprintf(stderr, "file not read need Bytes\n");
        return -1;
    }
    int ans = 0;
    size_t curByte1 = p1 / sizeUCharInBits;
    size_t curByte2 = p2 / sizeUCharInBits;
    size_t curBit1 = p1 % sizeUCharInBits;
    size_t curBit2 = p2 % sizeUCharInBits;
    for (int i = 0; i < n; ++i) {
        if (getBitFromByte(ptr[curByte1], curBit1) != getBitFromByte(ptr[curByte2], curBit2)) {
            ans++;
        }
        curBit1 = (curBit1 + 1) % sizeUCharInBits;
        curBit2 = (curBit2 + 1) % sizeUCharInBits;
        if (curBit1 == 0) {
            curByte1++;
        }
        if (curBit2 == 0) {
            curByte2++;
        }
    }
    fclose(file);
    free(ptr);
    printf("%d\n", ans);
    return 0;
}