//
// Created by ilya on 02.05.17.
//

#ifndef SERVER_WORKWITHMESSAGES_H
#define SERVER_WORKWITHMESSAGES_H

#include <sys/time.h>

#define MAX_CONNECTIONS 100
#define BUFF_SIZE 70000
#define NUM_THREADS 20
#define NAME_LEN 50
#define ERROR_CLOSE_SOCKET -111
#define LEN_FOR_TIME 8
#define STRING_LEN 1024
#define USER_ID_UNDEFINED 100000
#define MAX_MESSAGES_COUNT 10000
#define SMALL_BUFF 100
#define MAX_HISTORY_SEND 50

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

void writeIntToChars(char *buff, size_t x) {
    if (x != 0) {
        size_t y = 1;
        while (y <= x) {
            y *= 10;
        }
        y /= 10;
        int i;
        for (i = 0; y; ++i) {
            buff[i] = (char) ('0' + (x / y));
            x = x - y * (x / y);
            y /= 10;
        }
        buff[i] = 0;
    } else {
        buff[0] = '0';
        buff[1] = 0;
    }
}

void writeIntToStr(char *str, size_t x) {
    for (int i = 3; i >= 0; i--) {
        str[i] = (char)(x % 256);
        x /= 256;
    }
}

size_t readIntFromStr(char *string) {
    size_t res = 0;
    for (int i = 0; i < 4; ++i) {
        res = res * 256 + (string[i] + 256) % 256;
    }
    return res;
}

size_t readIntFromChars(char *str) {
    int i = 0;
    while (str[i] == ' ')
        ++i;
    size_t res = 0;
    for (i; '0' <= str[i] && str[i] <= '9'; ++i) {
        res = res * 10 + str[i] - '0';
    }
    return res;
}

ssize_t fullRecv(int sock, char* buffer, size_t count) {
    size_t bytesRead = 0;
    while (bytesRead != count) {
        ssize_t cntRead = recv(sock, buffer + bytesRead, count - bytesRead, 0);
        if (cntRead <= 0) {
            return cntRead;
        }
        bytesRead += cntRead;
    }
    return count;
}

ssize_t recvCommand(int sock, char* buffer) {
    ssize_t status = 0;
    if ((status = fullRecv(sock, buffer, 5)) <= 0) {
        return status;
    }
    size_t countBytes = readIntFromStr(buffer + 1);
    if (countBytes == 0)
        return 5;
    if ((status = fullRecv(sock, buffer + 5, countBytes)) <= 0) {
        return status;
    }
    return countBytes + 5;
}

size_t getCountStringsMessage(char *mess) {
    size_t bytesCount = readIntFromStr(mess + 1);
    mess += 5;
    size_t curBytesRead = 0;
    size_t countStrings = 0;
    while (curBytesRead != bytesCount) {
        size_t curLenString = readIntFromStr(mess + curBytesRead);
        curBytesRead += curLenString + 4;
        countStrings++;
    }
    return countStrings;
}

size_t getLenOfStringByIndex(char *mess, size_t ind) {
    mess += 5;
    size_t len = 0;
    for (int i = 0; i <= ind; ++i) {
        len = readIntFromStr(mess);
        mess += len + 4;
    }
    return len;
}

size_t getLenMessage(char *mess) {
    size_t lenBody = readIntFromStr(mess + 1);
    return lenBody + 5;
}

char* getStringByIndex(char *mess, size_t ind) {
    mess += 5;
    size_t len = 0;
    for (int i = 0; i < ind; ++i) {
        len = readIntFromStr(mess);
        mess += len + 4;
    }
    return mess + 4;
}

void setTypeMessage(char *mess, char c) {
    mess[0] = c;
}

void createEmptyMessage(char* mess) {
    writeIntToStr(mess + 1, 0);
}

void createMessageFromString(char* mess, char* str, size_t countBytes) {
    writeIntToStr(mess + 1, countBytes + 4);
    writeIntToStr(mess + 5, countBytes);
    for (int i = 0; i < countBytes; ++i) {
        mess[i + 9] = str[i];
    }
}

void addStringToMessage(char *mess, char* str, size_t countBytes) {
    size_t oldLen = readIntFromStr(mess + 1);
    writeIntToStr(mess + 1, oldLen + countBytes + 4);
    writeIntToStr(mess + oldLen + 5, countBytes);
    for (int i = 0; i < countBytes; ++i) {
        mess[i + oldLen + 9] = str[i];
    }
}

void getTime(char* timeStr) {
    struct timeval t;
    gettimeofday(&t, NULL);
    writeIntToStr(timeStr, (size_t)t.tv_sec);
    writeIntToStr(timeStr + 4, (size_t)t.tv_usec);
}

void createNotificationMessage(char* buffer, size_t ind, char* login, char* event) {
    char idToStr[SMALL_BUFF];
    writeIntToChars(idToStr, ind);
    char str[SMALL_BUFF]; str[0] = 0;
    strcat(str, "User with login: ");
    strcat(str, login);
    strcat(str, event);
    strcat(str, "chat!");
    setTypeMessage(buffer, 'm');
    char timeStr[LEN_FOR_TIME];
    getTime(timeStr);
    createMessageFromString(buffer, timeStr, 8);
    addStringToMessage(buffer, str, strlen(str));
}

void createNotificationLoginMessage(char* buffer, size_t ind, char* login) {
    createNotificationMessage(buffer, ind, login, " enter to ");
}

void createNotificationKickMessage(char* buffer, size_t ind, char* login) {
    createNotificationMessage(buffer, ind, login, " was kicked from ");
}

void createNotificationLogoutMessage(char* buffer, size_t ind, char* login) {
    createNotificationMessage(buffer, ind, login, " logout from ");
}

void createNotificationDisconnectingMessage(char* buffer, size_t ind, char* login) {
    createNotificationMessage(buffer, ind, login, " disconnected from ");
}

void createRegularMessage(char* buffer, char* login, char* body, size_t lenBody) {
    setTypeMessage(buffer, 'r');
    char timeStr[LEN_FOR_TIME];
    getTime(timeStr);
    createMessageFromString(buffer, timeStr, 8);
    addStringToMessage(buffer, login, strlen(login));
    addStringToMessage(buffer, body, lenBody);
}

void sendStatusMessage(int sock, size_t status) {
    char msg[100];
    msg[0] = 's';
    writeIntToStr(msg + 1, 8);
    writeIntToStr(msg + 5, 4);
    writeIntToStr(msg + 9, status);
    send(sock, msg, 13, 0);
}

void printString(char *str, size_t len) {
    for (int i = 0; i < len; ++i) {
        printf("%c", str[i]);
    }
}

size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

void printTime(time_t sec) {
    struct tm* timeInfo = localtime(&sec);
    printf("%d:%d%d ", timeInfo->tm_hour, (timeInfo->tm_min) / 10, timeInfo->tm_min % 10);
}

#endif //SERVER_WORKWITHMESSAGES_H
