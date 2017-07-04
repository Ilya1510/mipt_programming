#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#define MAX_LEN 4096

struct ProcessInfo {
    char command[MAX_LEN];
    char pid[MAX_LEN];
    char user[MAX_LEN];
    char fd[MAX_LEN];
    char name[MAX_LEN];
};

void printTitle() {
    printf("COMMAND      \tPID       \tUSER   \tFD   \tNAME\n");
}

void printOneFileInProcess(struct ProcessInfo* inf) {
    printf("%s       \t%s     \t%s     \t%s        \t%s\n", inf->command, inf->pid, inf->user, inf->fd, inf->name);
}

void fillCommand(struct ProcessInfo* inf) {
    const char* pidStr = inf->pid;
    char comm[MAX_LEN];
    char pathToCommand[MAX_LEN];
    sprintf(pathToCommand, "/proc/%s/comm", pidStr);
    FILE* file = fopen(pathToCommand, "r");
    fscanf(file, "%s", comm);
    for (int i = 0; i < MAX_LEN && comm[i] != 0; ++i) {
        if (comm[i] == ':') {
            comm[i] = 0;
        }
    }
    fclose(file);
    strcpy(inf->command, comm);
}

void fillOwner(struct ProcessInfo* inf) {
    const char* pidStr = inf->pid;
    struct stat statInfo;
    char pathToDirProc[MAX_LEN];
    sprintf(pathToDirProc, "/proc/%s", pidStr);
    stat(pathToDirProc, &statInfo);
    struct passwd* userInfo = getpwuid(statInfo.st_uid);
    if (userInfo != NULL) {
        strcpy(inf->user, userInfo->pw_name);
    } else {
        strcpy(inf->user, "(getpwuid faild)");
    }
}

void printProcess(struct dirent* dir) {
    struct ProcessInfo inf;
    strcpy(inf.command, "***");
    strcpy(inf.pid, "***");
    strcpy(inf.user, "***");
    strcpy(inf.fd, "***");
    strcpy(inf.name, "***");

    const char* pidStr = dir->d_name;
    strcpy(inf.pid, pidStr);

    fillCommand(&inf);
    fillOwner(&inf);

    char pathToFD[MAX_LEN];
    sprintf(pathToFD, "/proc/%s/fd", pidStr);
    DIR* dirp = opendir(pathToFD);
    if (dirp == NULL) {
        strcpy(inf.name, strerror(errno));
    } else {
        while (1) {
            //errno = 0;
            struct dirent* de = readdir(dirp);
            if (!de) {
                if (errno) {
                    strcpy(inf.name, strerror(errno));
                    //printOneFileInProcess(&inf);
                }
                break;
            }
            if (de->d_name[0] == '.') {
                continue;
            }
            char link[MAX_LEN];
            snprintf(link, sizeof(link), "/proc/%s/fd/%s", pidStr, de->d_name);
            char name[MAX_LEN];
            ssize_t nname = readlink(link, name, sizeof(name) - 1);
            if (nname < 0) {
                snprintf(name, sizeof(name), "(readlink failed: %s)", strerror(errno));
            } else {
                name[nname] = 0;
            }
            strcpy(inf.fd, de->d_name);
            strcpy(inf.name, name);
            printOneFileInProcess(&inf);
        }
    }
    closedir(dirp);
}

//returns 1 if number and 0 - else
int isNumber(char *str) {
    int allCharsIsDigits = 1;
    for (int i = 0; str[i] != 0; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            allCharsIsDigits = 0;
        }
    }
    return allCharsIsDigits;
}

int main(int argc, char** argv) {
    errno = 0;

    printTitle();
    DIR* dirAllProc = opendir("/proc");
    if (dirAllProc == NULL) {
        perror("Can't open /proc\n");
        return -1;
    }

    while (1) {
        errno = 0;
        struct dirent* currentProcess = readdir(dirAllProc);
        if (currentProcess != NULL) {
            if (isNumber(currentProcess->d_name) == 1) {
                printProcess(currentProcess);
            }
        } else {
            if (errno) {
                perror("Can't open next dir\n");
                closedir(dirAllProc);
                return -1;
            }
            break;
        }
    }
    closedir(dirAllProc);
    return 0;
}
