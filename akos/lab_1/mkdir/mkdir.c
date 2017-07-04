#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <getopt.h>

//0 - all, 1 - without EEXIST
int tryMakeDir(char* path, int notReactErrors) {
    int notReactExist = notReactErrors & 1;
    int status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == -1) {
        if (errno == EEXIST && notReactExist) {
            return 0;
        }
        return -1;
    }
    return 0;
}

void printErr(char* path) {
    if (errno == EACCES) {
        printf("mkdir: not allow to create '%s'\n", path);
    } else if (errno == EEXIST) {
        printf("mkdir: cannot create directory ‘%s’: File exists\n", path);
    } else if (errno == ENAMETOOLONG) {
        printf("'%s' : File name too long\n", path);
    } else if (errno == ENOTDIR) {
        printf("mkdir: cannot create directory ‘%s’: It is not a directory\n", path);
    } else if (errno == ENOENT) {
        printf("mkdir: A directory component '%s' in pathname does not exist\n", path);
    }
}

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("mkdir: missing operand\n");
        printf("Try 'mkdir --help' for more information.\n");
    }
    int pFlag = 0;
    int c;
    while ((c = getopt(argc, argv, "p")) != -1) {
        switch (c) {
            case 'p':
                pFlag = 1;
                break;

            case '?':
                return -1;

            default:
                continue;
        }
    }
    int workWithoutErrors = 0;
    for (int i = optind; i < argc; ++i) {
        if (pFlag == 1) {
            // -p is active
            // without EEXIST
            for (int j = 1; j < strlen(argv[i]); ++j) {
                if (argv[i][j] == '/') {
                    argv[i][j] = 0;
                    int status = tryMakeDir(argv[i], 1);
                    if (status == -1) {
                        printErr(argv[i]);
                        workWithoutErrors = 1;
                        break;
                    }
                    argv[i][j] = '/';
                }
            }
            if (workWithoutErrors == 1) {
                break;
            }
        }
        int status = tryMakeDir(argv[i], 0);
        if (status == -1) {
            printErr(argv[i]);
            workWithoutErrors = 1;
            continue;
        }
    }
    return workWithoutErrors;
}
