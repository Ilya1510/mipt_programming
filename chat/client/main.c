#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>
#include "../server/workWithMessages.h"

pthread_t pthreadRecv;
int PORT = 1337;

size_t fillLoginMessage(char *msg, char *login, char *password) {
    setTypeMessage(msg, 'i');
    createMessageFromString(msg, login, strlen(login));
    addStringToMessage(msg, password, strlen(password));
    return getLenMessage(msg);
}

//! fill id and reason from line
void parseKickMessage(char* line, size_t* id, char* reason) {
    //"/kick <size_t id> <char* reason>
    *id = readIntFromChars(line + 5);
    size_t i = 0;
    while (!isDigit(line[i])) {
        ++i;
    }
    while (isDigit(line[i])) {
        ++i;
    }
    while (line[i] == ' ') {
        ++i;
    }
    strcpy(reason, line + i);
}

void writeStatus(size_t status) {
    if (status == 0) {
        printf("OK\n");
    } else if (status == 3) {
        printf("ошибка аутентификации\n");
    } else if (status == 2) {
        printf("Незалогиненный пользователь\n");
    } else if (status == 6) {
        printf("Invalid message\n");
    } else if (status == 5) {
        printf("Ошибка доступа\n");
    } else {
        printf("status: %d\n", (int)status);
    }
}

void doLogin(int sockId) {
    printf("Введите Логин и пароль:\n");
    printf("login: ");

    size_t tmpSize;
    char *userName = malloc(NAME_LEN * sizeof(char));
    getline(&userName, &tmpSize, stdin);
    userName[strlen(userName) - 1] = 0;
    printf("password: ");
    char *userPassword = malloc(NAME_LEN * sizeof(char));
    getline(&userPassword, &tmpSize, stdin);
    userPassword[strlen(userPassword) - 1] = 0;

    char *sendMsgBuff = malloc(sizeof(char) * BUFF_SIZE);
    size_t countBytes = fillLoginMessage(sendMsgBuff, userName, userPassword);
    if (send(sockId, sendMsgBuff, countBytes, 0) != countBytes) {
        printf("couldn't send message to login\n");
    }
}

void processMessageM(char* mess) {
    char* timeStr = getStringByIndex(mess, 0);
    size_t sec = readIntFromStr(timeStr);
    //size_t mic = readIntFromStr(timeStr + 4);
    printTime(sec);
    printString(getStringByIndex(mess, 1), getLenOfStringByIndex(mess, 1));
    printf("\n");
}

void processMessageKick(char* mess) {
    printf("I'm kicked with reason: ");
    printString(getStringByIndex(mess, 0), getLenOfStringByIndex(mess, 0));
    printf("\n");
}

void processMessageHistory(char* mess) {
    size_t messCount = getCountStringsMessage(mess) / 3;
    printf("Кол-во сообщений: %ld\n", messCount);
    for (size_t i = 0; i < messCount; ++i) {
        char *timestamp = getStringByIndex(mess, 3 * i);
        printf("time-");
        size_t sec = readIntFromStr(timestamp);
        //size_t mic = readIntFromStr(timestamp + 4);
        printTime(sec);
        char *login = getStringByIndex(mess, 3 * i + 1);
        char *body = getStringByIndex(mess, 3 * i + 2);
        printString(login, getLenOfStringByIndex(mess, 3 * i + 1));
        printf(" ");
        printString(body, getLenOfStringByIndex(mess, 3 * i + 2));
        printf("\n");
    }
}

void processMessageList(char* mess) {
    size_t usersCount = getCountStringsMessage(mess) / 2;
    printf("Кол-во пользователей:%ld\n", usersCount);
    for (size_t i = 0; i < usersCount; ++i) {
        size_t userId = readIntFromStr(getStringByIndex(mess, 2 * i));
        char *login = getStringByIndex(mess, 2 * i + 1);
        printf("%ld ", userId);
        for (int j = 0; j < getLenOfStringByIndex(mess, 2 * i + 1); ++j) {
            printf("%c", login[j]);
        }
        printf("\n");
    }
}

void processMessageRegular(char * mess) {
    char *timeStr = getStringByIndex(mess, 0);
    time_t sec = readIntFromStr(timeStr);
    //size_t mic = readIntFromStr(timeStr + 4);
    printTime(sec);
    printf("from user: %s ", getStringByIndex(mess, 1));
    printf("message: "); printString(getStringByIndex(mess, 2),
                                     getLenOfStringByIndex(mess, 2));
    printf("\n");
}

void* listenFromServer(void * data) {
    int serverSock = *(int*)data;
    char buffer[BUFF_SIZE];
    while (1) {
        ssize_t count = recvCommand(serverSock, buffer);
        if (count <= 0) {
            fprintf(stderr, "Прервано соединение с сервером\n");
            close(serverSock);
            _exit(-1);
            return NULL;
        }
        if (buffer[0] == 's') {
            writeStatus(readIntFromStr(buffer + 9));
        }
        if (buffer[0] == 'r') {
            processMessageRegular(buffer);
        }
        if (buffer[0] == 'l') {
            processMessageList(buffer);
        }
        if (buffer[0] == 'h') {
            processMessageHistory(buffer);
        }
        if (buffer[0] == 'm') {
            processMessageM(buffer);
        }
        if (buffer[0] == 'k') {
            processMessageKick(buffer);
        }
    }
}

void sendQueryHistory(int sock, size_t count) {
    char buffer[SMALL_BUFF];
    setTypeMessage(buffer, 'h');
    char cntString[4];
    writeIntToStr(cntString, count);
    createMessageFromString(buffer, cntString, 4);
    ssize_t cntSend = send(sock, buffer, getLenMessage(buffer), 0);
    if (cntSend != getLenMessage(buffer)) {
        printf("Can't send history message\n");
    }
}

void sendQueryKick(int sock, size_t id, char *reason) {
    char buffer[SMALL_BUFF];
    setTypeMessage(buffer, 'k');
    char idString[4];
    writeIntToStr(idString, id);
    createMessageFromString(buffer, idString, 4);
    addStringToMessage(buffer, reason, strlen(reason));
    ssize_t cntSend = send(sock, buffer, getLenMessage(buffer), 0);
    if (cntSend != getLenMessage(buffer)) {
        printf("Can't send kick message\n");
    }
}

void processCommand(int sock, char *line) {
    char buff[100];
    if (!strcmp(line, "/list")) {
        createEmptyMessage(buff);
        setTypeMessage(buff, 'l');
        send(sock, buff, 5, 0);
        return;
    } else if (!strcmp(line, "/logout")) {
        createEmptyMessage(buff);
        setTypeMessage(buff, 'o');
        send(sock, buff, 5, 0);
        return;
    } else if (!strcmp(line, "/login")) {
        doLogin(sock);
        return;
    }  else if (!strncmp(line, "/history", 8)) {
        size_t count = readIntFromChars(line + 8);
        sendQueryHistory(sock, count);
        return;
    } else if (!strncmp(line, "/kick", 5)) {
        size_t userKickId;
        char reason[SMALL_BUFF];
        parseKickMessage(line, &userKickId, reason);
        sendQueryKick(sock, userKickId, reason);
        return;
    } else if (line[0] != '/') {
        char buffer[BUFF_SIZE];
        setTypeMessage(buffer, 'r');
        createMessageFromString(buffer, line, strlen(line));
        send(sock, buffer, getLenMessage(buffer), 0);
    } else {
        printf("Unknown command\n");
    }
}

int main(int argc, char** argv) {
    int opt;
    char ipAddr[20];
    strcpy(ipAddr, "127.0.0.1");
    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
        switch (opt) {
            case 'i':
                strcpy(ipAddr, optarg);
                break;
            case 'p':
                PORT = atoi(optarg);
                break;
            default:
                break;
        }
    }
    int clientSock;
    if ((clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Socket opening failed\n");
        return 1;
    }
    struct sockaddr_in port;
    port.sin_family = AF_INET;
    port.sin_port = htons((uint16_t)PORT);
    port.sin_addr.s_addr = inet_addr(ipAddr);
    if (connect(clientSock, (struct sockaddr *) &port, sizeof(port)) < 0) {
        fprintf(stderr, "Connect to server failed\n");
        return 2;
    }
    printf("Теперь вы можете писать и получать сообщения\n");
    printf("Возможные команды: /login /logout /history /list /kick\n");
    pthread_create(&pthreadRecv, NULL, listenFromServer, (void *)&clientSock);
    ssize_t read;
    size_t len = 0;
    char* line = NULL;
    while ((read = getline(&line, &len, stdin)) != -1) {
        line[read - 1] = 0;
        processCommand(clientSock, line);
    }
    return 0;
}