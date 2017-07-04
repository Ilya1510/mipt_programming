#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int isConsistPattern(char* str, char* pattern, int ignoreCase) {
    int res = 0;
    size_t n = strlen(str);
    size_t m = strlen(pattern);
    for (int i = 0; i + m <= n; ++i) {
        char* begin = str + i;
        int stringEquals = 0;
        if (ignoreCase == 0) {
            stringEquals = strncmp(begin, pattern, m) == 0;
        } else {
            stringEquals = strncasecmp(begin, pattern, m) == 0;
        }
        res |= stringEquals;
    }
    return res;
}

int main(int argc, char** argv) {
    int iFlag = 0;
    int vFlag = 0;
    char *pattern = NULL;
    char* file = NULL;
    int c;
    while ((c = getopt(argc, argv, "ivf:")) != -1) {
        switch (c) {
            case 'i':
                iFlag = 1;
                break;

            case 'v':
                vFlag = 1;
                break;

            case 'f':
                file = optarg;
                break;

            case 'e':
                pattern = optarg;
                break;

            case '?':
                return 2;

            default:
                continue;
        }
    }
    if (pattern == NULL) {
        if (optind == argc) {
            printf("use grep [OPTIONS] PATTERN [FILE...]\n");
            return 2;
        }
        pattern = argv[optind++];
    }
    char* strBuff = NULL;
    //Read from stdin
    if (file == NULL && optind == argc) {
        int countFindString = 0;
        size_t n = 0;
        while (getline(&strBuff, &n, stdin) != EOF) {
            int hasPattern = isConsistPattern(strBuff, pattern, iFlag);
            if (vFlag ^ hasPattern == 1) {
                printf("%s", strBuff);
                ++countFindString;
            }
        }
        free(strBuff);
        return countFindString == 0;
    }

    //Read from Files
    if (file == NULL) {
        file = argv[optind++];
    }
    int isAnyFiles = 0;
    if (optind != argc) {
        isAnyFiles = 1;
    }
    int countFindString = 0;
    while (1) {
        FILE* openFile;
        if ((openFile = fopen(file, "r")) == NULL) {
            printf("grep: %s:%s\n", file, strerror(errno));
        } else {
            size_t n = 0;
            while (getline(&strBuff, &n, openFile) != EOF) {
                int hasPattern = isConsistPattern(strBuff, pattern, iFlag);
                if (vFlag ^ hasPattern == 1) {
                    if (isAnyFiles) {
                        printf("%s:", file);
                    }
                    printf("%s", strBuff);
                    ++countFindString;
                }
            }
        }
        fclose(openFile);
        if (optind == argc) {
            break;
        }
        file = argv[optind++];
    }
    free(strBuff);
    fclose(stdin);
    return countFindString == 0;
}